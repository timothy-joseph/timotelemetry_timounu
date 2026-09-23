#include <stdio.h>
#include <string.h>

#include <esp_log.h>
#include <esp_check.h>
#include <driver/gpio.h>
#include <driver/uart.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <lora_driver.h>

#define LORA_LOG_TAG "lora_driver"

/* defined here to changing between normal mode and TEST mode hex format is
 * easy. (AT_NORMAL_MODE or AT_TEST_MODE should be defined in lora_driver.h")
 */
#ifdef AT_NORMAL_MODE
#define AT_HEXMSG_FMT "AT+MSGHEX=\"%s\"\r\n"
#define AT_HEXMSG_EXPECTED "+MSGHEX: Done"
#define AT_RXMSG_DETECT "RX: \""
#endif

#ifdef AT_TEST_MODE
#define AT_HEXMSG_FMT "AT+TEST=TXLRPKT, \"%s\"\r\n"
#define AT_HEXMSG_EXPECTED "+TEST: TX DONE"
#define AT_RXMSG_DETECT "+TEST: RX"
#endif

static int lora_send_await_response(struct lora_dev_t *handle,
				    char *expected_response,
				    char *command, ...);
static int lora_config_via_at_commands(struct lora_dev_t *handle);
static int lora_nrmlcfg_via_at_commands(struct lora_dev_t *handle);
static int lora_testcfg_via_at_commands(struct lora_dev_t *handle);
static int lora_config_via_at_cmds_recv(struct lora_dev_t *handle);
static void lora_received_response_cb(struct lora_dev_t *handle, char *resp);
static void lora_message_cb(struct lora_dev_t *handle, char *resp);
static inline char __int_to_hex_digit(uint8_t digit);
static inline uint8_t __hex_to_int_digit(char digit);
static inline uint8_t __is_hex_digit(char digit);
static void lora_uart_rx_thread(void *args);
static void lora_sender_thread(void *args);

int
lora_init_uart(struct lora_dev_t *handle, uart_port_t uart_num,
	       gpio_num_t tx_pin, gpio_num_t rx_pin, uint8_t rx_thread_prio,
	       TickType_t universal_timeout)
{
	int ret;
	uart_config_t uart_cfg = {
		.baud_rate = 9600,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	};

	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
			    "handle is NULL inside of lora_init_uart\r\n");

	/* initialize the structure (should already be initialized) */
	handle->uart_num = uart_num;
	handle->universal_timeout = universal_timeout;
	handle->received_messages = NULL;

	/* create the response mutex (used by the uart event thread to safely
	 * change the latest response)
	 */
	handle->resp_mutex = xSemaphoreCreateMutex();
	if (!handle->resp_mutex)
		return ESP_FAIL;

	/* initialize the uart driver */
	ESP_RETURN_ON_ERROR(uart_param_config(uart_num, &uart_cfg),
			    LORA_LOG_TAG, "uart_config fail\r\n");
	ESP_RETURN_ON_ERROR(uart_set_pin(uart_num, tx_pin, rx_pin,
					 UART_PIN_NO_CHANGE,
					 UART_PIN_NO_CHANGE),
			    LORA_LOG_TAG, "uart_set_pin fail\r\n");
	ESP_RETURN_ON_ERROR(uart_driver_install(uart_num, 2048, 2048, 10,
					    &handle->uart_event_queue, 0),
			    LORA_LOG_TAG, "uart_driver_install fail\r\n");

	/* create uart event thread */
	ret = xTaskCreatePinnedToCore(lora_uart_rx_thread,
				      "lora_uart_rx_thread", 4096, NULL,
				      rx_thread_prio, NULL, tskNO_AFFINITY);
	if (ret != pdPASS) {
		ESP_LOGE(
		  LORA_LOG_TAG,
		  "Failed to create the thread for receiving uart events\r\n"
		  	);
		vSemaphoreDelete(handle->resp_mutex);
		return ESP_FAIL;
	}

	return lora_config_via_at_commands(handle);
}

int
lora_init_as_sender(struct lora_dev_t *handle, size_t queue_len,
		    int thread_priority)
{
	int ret;

	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
			    "handle is NULL inside of lora_init_as_sender\r\n");

	/* check if commands can be sent, if they can be sent, then configure
	 * the module as a sender
	 */
	handle->payload_queue = xQueueCreate(queue_len,
					     sizeof(struct lora_payload_t));
	ESP_RETURN_ON_FALSE(handle->payload_queue, ESP_FAIL, LORA_LOG_TAG,
			    "Failed to create the queue for the payloads\r\n");

	ret = xTaskCreatePinnedToCore(lora_sender_thread, "lora_sender_thread",
				      4096, handle, thread_priority, NULL,
				      tskNO_AFFINITY);
	if (ret != pdPASS) {
		ESP_LOGW(LORA_LOG_TAG,
			 "Failed to create the thread for sending queued payloads\r\n");
		vQueueDelete(handle->payload_queue);
		return ESP_FAIL;
	}

	return ESP_OK;
}

int
lora_init_as_receiver(struct lora_dev_t *handle, size_t msg_queue_len)
{
	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
			  "handle is NULL inside of lora_init_as_receiver\r\n");

	handle->received_messages = xQueueCreate(msg_queue_len,
						 sizeof(struct lora_message_t));
	ESP_RETURN_ON_FALSE(handle->received_messages, ESP_FAIL, LORA_LOG_TAG,
			    "Failed to create the queue for the messages\r\n");

	return lora_config_via_at_cmds_recv(handle);
}

int
lora_enqueue_command(struct lora_dev_t *handle, char *expected_response,
		     char *command, ...)
{
	struct lora_payload_t to_enq;
	va_list args;

	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
			    "handle is NULL inside of lora_enqueue_command\r\n"
			   );
	ESP_RETURN_ON_FALSE(command, ESP_FAIL, LORA_LOG_TAG,
			    "command is NULL inside of lora_enqueue_command\r\n"
			   );

	va_start(args, command);
	vsnprintf(to_enq.command, sizeof(to_enq.command), command, args);
	va_end(args);

	if (expected_response)
		strncpy(to_enq.expected_response, expected_response,
			sizeof(to_enq.expected_response));
	else
		to_enq.expected_response[0] = '\0';
	to_enq.expected_response[sizeof(to_enq.expected_response) - 1] = '\0';

	if (xQueueSend(handle->payload_queue, &to_enq,
		       handle->universal_timeout) != pdTRUE) {
		ESP_LOGW(LORA_LOG_TAG, "Couldn't add item to queue\r\n");
		return ESP_FAIL;
	}

	return ESP_OK;
}

int
lora_enqueue_hexmsg(struct lora_dev_t *handle, uint8_t *message,
		    size_t message_size)
{
	char hex_msg[LORA_COMMAND_LENGTH];
	size_t hex_msg_len, i;

	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
			    "handle is NULL inside of lora_enqueue_hexmsg\r\n"
			   );
	ESP_RETURN_ON_FALSE(message, ESP_FAIL, LORA_LOG_TAG,
			    "message is NULL inside of lora_enqueue_hexmsg\r\n"
			   );

	/* translate each byte of message into two bytes representing the hex
	 * value of that byte and put it at the end of hex_msg. hex_msg is
	 * null terminated
	 */
	memset(hex_msg, 0, sizeof(hex_msg));
	hex_msg_len = 0;
	for (i = 0; i < message_size && hex_msg_len < sizeof(hex_msg); i++) {
		hex_msg[hex_msg_len] = __int_to_hex_digit(message[i] >> 4);
		if (hex_msg_len + 1 >= sizeof(hex_msg))
			break;
		hex_msg[hex_msg_len + 1] = __int_to_hex_digit(message[i] & 0xf);
		hex_msg_len += 2;
	}
	hex_msg[sizeof(hex_msg) - 1] = '\0';

	return lora_enqueue_command(handle, AT_HEXMSG_EXPECTED,
				    AT_HEXMSG_FMT, hex_msg);
}

/**
 * Internal function used by the driver to send a command formated using the
 * printf format style and wait/block until the expected_response is received
 * or until the timeout specified inside of handle expires
 *
 * @param handle The initialized handle to a lora device
 * @param expected_response The ok message that is expected (null terminated
 *			    string)
 * @param command A formated string representing the command in printf format
 *		  (null terminated string)
 * @param ... The format information
 * @return ESP_OK on success (if all of the parameters were valid and the
 *		  expected response was received)
 *	   ESP_FAIL otherwise
 */
static int
lora_send_await_response(struct lora_dev_t *handle, char *expected_response,
			 char *command, ...)
{
	/* This buffer will be used for both sending and receiving messages */
	char cmd[LORA_COMMAND_LENGTH], *p;
	size_t cmd_len;
	uint8_t received_response;
	va_list args;
	TickType_t start_ticks;

	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
		       "handle is NULL inside of lora_send_await_response\r\n");

	/* make the command and send it */
	va_start(args, command);
	cmd_len = vsnprintf(cmd, sizeof(cmd), command, args);
	va_end(args);
	if (cmd_len >= sizeof(cmd))
		cmd_len = sizeof(cmd) - 1;
	uart_write_bytes(handle->uart_num, cmd, cmd_len);

	if (expected_response == NULL || *expected_response == '\0')
		return ESP_OK;

	/* poll for the first response (or until the timeout expires). this
	 * could be done more cleanly (maybe separate the following lines into
	 * a new function)
	 */

	start_ticks = xTaskGetTickCount();

	if (xSemaphoreTake(handle->resp_mutex,
			   handle->universal_timeout) != pdTRUE)
		return ESP_FAIL;
	handle->received_response = 0;
	xSemaphoreGive(handle->resp_mutex);
	received_response = 0;
	do {
		if (xSemaphoreTake(handle->resp_mutex,
				   handle->universal_timeout) != pdTRUE)
			continue;
		if (handle->received_response)
			received_response = 1;
		xSemaphoreGive(handle->resp_mutex);
	} while (!received_response &&
		 xTaskGetTickCount() - start_ticks <=
		 handle->universal_timeout);

	if (!received_response)
		return ESP_FAIL;

	if (xSemaphoreTake(handle->resp_mutex,
			   handle->universal_timeout) != pdTRUE)
		return ESP_FAIL;
	p = strstr(handle->last_response, expected_response);
	xSemaphoreGive(handle->resp_mutex);

	if (p)
		return ESP_OK;

	return ESP_FAIL;
}

/**
 * Function used to configure the module (not customizable outside of the
 * driver). YOU MUST EDIT THIS TO MATCH YOUR SETUP.
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
static int
lora_config_via_at_commands(struct lora_dev_t *handle)
{
	ESP_RETURN_ON_FALSE(handle, ESP_FAIL, LORA_LOG_TAG,
	         "handle is NULL inside of lora_config_via_at_commands\r\n");
#ifdef AT_TEST_MODE
	return lora_testcfg_via_at_commands(handle);
#endif
#if !defined(AT_TEST_MDOE) && !defined(AT_NORMAL_MODE)
	ESP_LOGW(LORA_LOG_TAG,
		 "lora_config_via_at_commands: No mode defined\r\n");
#endif
	return lora_nrmlcfg_via_at_commands(handle);
}

/**
 * Configuration for when in normal mode (check lora at command specification)
 * THESE OPTIONS WILL NEED TO BE CHANGED TO MEET THE AREA THAT THE DEVICE IS
 * USED IN
 *
 * @return esp_ok on success
 *	   esp_fail otherwise
 */
static int
lora_nrmlcfg_via_at_commands(struct lora_dev_t *handle)
{
	/* TODO: check if the configutation is good */
	if (lora_send_await_response(handle, "+AT: OK", "AT\r\n") != ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "+AT: OK failed\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response(handle, "+ID: AppEui", "AT+ID\r\n") !=
	    ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "ID failed during config\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response
	    (handle, "+MODE: LWOTAA", "AT+MODE=LWOTAA\r\n") != ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "MODE failed during config\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response(handle, "+DR: EU868", "AT+DR=EU868\r\n") !=
	    ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "DR failed during config\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response(handle, "+CH: NUM", "AT+CH=NUM,0-2\r\n") !=
	    ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "CH failed during config\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response (handle, "+KEY: APPKEY",
	     "AT+KEY=APPKEY,\"2B7E151628AED2A6ABF7158809CF4F3C\"\r\n") !=
	    ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "KEY failed during config\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response(handle, "+CLASS: C", "AT+CLASS=A\r\n") !=
	    ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "CLASS failed during config\r\n");
		return ESP_FAIL;
	}
	if (lora_send_await_response(handle, "+PORT: 8", "AT+PORT=8\r\n") !=
	    ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "PORT failed during config\r\n");
		return ESP_FAIL;
	}

	return ESP_OK;
}

/**
 * Configuration for when in TEST mode (check lora at command specification)
 * THESE OPTIONS WILL NEED TO BE CHANGED TO MEET THE AREA THAT THE DEVICE IS
 * USED IN
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
static int
lora_testcfg_via_at_commands(struct lora_dev_t *handle)
{
	if (lora_send_await_response(handle, "+MODE: TEST", "AT+MODE=TEST\r\n")
	    != ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "MODE failed during config\r\n");
		return ESP_FAIL;
	}
	/* TODO: check if these options are good */
	if (lora_send_await_response(handle, "+TEST: RFCFG",
	    "AT+TEST=RFCFG,866,SF12,125,12,15,14,ON,OFF,OFF\r\n") != ESP_OK) {
		ESP_LOGW(LORA_LOG_TAG, "TEST: RFCFG failed during config\r\n");
		return ESP_FAIL;
	}

	return ESP_OK;
}

static int
lora_config_via_at_cmds_recv(struct lora_dev_t *handle)
{
	/* TODO: i think only the test mode has to be configured seperatly for
	 * receiving, but i need to check
	 */
#if defined(AT_TEST_MODE)
	return lora_send_await_response(handle, "+TEST:", "AT+TEST=RXLRPKT\r\n");
#elif defined(AT_NORMAL_MODE)
#endif
	return ESP_FAIL;
}

/**
 * Function that is called when a response was received.
 * resp contains something like "\r\n.*\0". If the uart response received is a
 * lora message call the appropriate call back function, otherwise take the
 * handle->resp_mutex and set handle->received_response and
 * handle->last_response
 */
static void
lora_received_response_cb(struct lora_dev_t *handle, char *resp)
{
	if (strstr(resp, AT_RXMSG_DETECT)) {
		lora_message_cb(handle, resp);
		return;
	}

	/* response was recevied, not message */
	if (xSemaphoreTake(handle->resp_mutex,
			   handle->universal_timeout) != pdTRUE)
		return;
	handle->received_response = 1;
	strncpy(handle->last_response, resp, sizeof(handle->last_response));
	handle->last_response[sizeof(handle->last_response) - 1] = '\0';
	xSemaphoreGive(handle->resp_mutex);
}

/**
 * The callback function for receiving a message. THE MESSAGE IS ASSUMED TO BE
 * HEX FORMAT
 * the response is assumed to be null terminated
 */
static void
lora_message_cb(struct lora_dev_t *handle, char *resp)
{
	/* i implemented the AT+TEST=RXLRPKT format */
	char *p;
	uint8_t byte;
	struct lora_message_t message;

	message.size = 0;

	p = strstr(resp, AT_RXMSG_DETECT);
	if (!p)
		return;

	if (p - resp + strlen(AT_RXMSG_DETECT) >= strlen(resp))
		return;
	p += strlen(AT_RXMSG_DETECT);
	
	while (*p && p[1] && message.size < LORA_MESSAGE_LENGTH) {
		if (*p == ' ') {
			p++;
			continue;
		}

		if (!__is_hex_digit(*p) || !__is_hex_digit(p[1]))
			break;
		byte = (__hex_to_int_digit(*p) << 4) | __hex_to_int_digit(p[1]);
		message.data[message.size++] = byte;
		p += 2;
	}

	xQueueSend(handle->received_messages, &message,
		   handle->universal_timeout);
}

/**
 * Internal function used to translate a number between 0 and 15 into its
 * corresponding digit (into a char '0' - '9', 'A' - 'F')
 */
static inline char
__int_to_hex_digit(uint8_t digit)
{
	return digit < 10 ? '0' + digit : 'A' + digit - 10;
}

/**
 * Internal function used to translate a hex digit ('0' - '9', 'A' - 'F' into a
 * number)
 */
static inline uint8_t
__hex_to_int_digit(char digit)
{
	if (digit >= '0' && digit <= '9')
		return digit - '0';
	if (digit >= 'A' && digit <= 'F')
		return digit - '0';
	return 0;
}

/**
 * Internal function used to determine if a char is a hexdigit ('0' - '9', 'A'
 * - 'F')
 */
static inline uint8_t
__is_hex_digit(char digit)
{
	return (digit >= '0' && digit <= '9') || (digit >= 'A' && digit <= 'F');
}

/**
 * Thread that receives data from based on the uart event queue. When a full
 * response is received "\r\n.*\r\n", then the appropriate callback function is
 * called
 *
 * @param args struct lora_dev_t *args, which contains the uart port number
 */
static void
lora_uart_rx_thread(void *args)
{
	struct lora_dev_t *lora_dev_handle;
	char buff[LORA_RESPONSE_LENGTH], response[sizeof(buff)], *p;
	size_t buff_len, read_len, buff_resp_start, buff_resp_end;
	uart_event_t event;

	lora_dev_handle = (struct lora_dev_t *)args;

	buff_len = 0;

	while (1) {
		if (!xQueueReceive(lora_dev_handle->uart_event_queue, &event,
				   portMAX_DELAY))
			continue;
		read_len = uart_read_bytes(lora_dev_handle->uart_num,
					   buff + buff_len,
					   sizeof(buff) - buff_len - 1, 0);
		buff_len += read_len;
		buff[buff_len] = 0;

		/* check to see if the pattern "\r\n.*\r\n" if found within the
		 * buffer. if it is, then copy just it into response_buff and
		 * call the parse function which will call the appropriate
		 * callback function
		 *
		 * TODO: check if these string manipulations are safe
		 */
		p = strstr(buff, "\r\n");
		if (!p)
			continue;
		buff_resp_start = p - buff;
		if (buff_resp_start >= sizeof(buff) - 2) {
			buff_len = 0;
			continue;
		}

		p = strstr(p + 1, "\r\n");
		if (!p)
			continue;
		buff_resp_end = p - buff;

		memset(response, 0, sizeof(response));
		strncpy(response, buff + buff_resp_start,
			buff_resp_end - buff_resp_start + 1);

		if (buff_resp_end + 2 < sizeof(buff)) {
			strncpy(buff, buff + buff_resp_end + 2,
				buff_len - buff_resp_end - 2);
			buff_len -= buff_resp_end + 2;
			continue;
		}

		lora_received_response_cb(lora_dev_handle, response);

		if (buff_len >= sizeof(buff) - 1) {
			buff_len = 0;
			continue;
		}
	}

}

/**
 * Thread that listens to the queue inside of args for commands to send and
 * await a response to.
 *
 * @param args struct lora_dev_t *args, which contains the uart port number and
 *	       the queue. The uart port number and queue may not be changed
 *	       after starting this thread.
 */
static void
lora_sender_thread(void *args)
{
	struct lora_dev_t *lora_dev_handle;
	struct lora_payload_t payload;
	int ret;

	lora_dev_handle = (struct lora_dev_t *)args;

	if (lora_dev_handle == NULL) {
		ESP_LOGE(LORA_LOG_TAG, "handler is null in thread\r\n");
		vTaskSuspend(NULL);
	}

	while (1) {
		if (!xQueueReceive(lora_dev_handle->payload_queue, &payload,
				   portMAX_DELAY))
			continue;

		ret = lora_send_await_response(lora_dev_handle,
					       payload.expected_response,
					       payload.command);
		if (ret != ESP_OK)
			ESP_LOGW(LORA_LOG_TAG,
			    "Sending %s didn't receive expected response: %s\n",
			    payload.command, payload.expected_response);
	}
}

