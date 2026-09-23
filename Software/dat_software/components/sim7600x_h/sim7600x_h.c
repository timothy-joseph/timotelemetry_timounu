/*
 * File structure
 * 1. extern functions:
 *	- init uart driver
 *	- setup module
 *	- enqueue commands
 * 2. static functions
 *	- await response
 *	- mutex functions (maybe i should remove these because they are just
 *	wrappers for FreeRTOS functions)
 *	- threads
 *	- callbacks for responses (gps, clock, connlost)
 *	- misc
 */
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <ctype.h>

#include <esp_log.h>
#include <esp_check.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include "driver/twai.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <rtc_max31331.h>
#include <logging.h>

#include "sim7600x_h.h"

#include "sim7600x_h_config.h"

static int sim_await_response(char *expected_response);
static int sim_init_rtc(void);
static void sim_cclk_extract_info(char *resp);
static int sim_take_mutex(void);
static void sim_give_mutex(void);
static int sim_send_data_await_resp_unsafe(char *expected_response,
					   uint8_t *data, size_t data_size);

static SemaphoreHandle_t sim_mutex;

int
sim_init_uart(void)
{
	/* init uart driver */
	uart_config_t uart_cfg = {
		.baud_rate = SIM_UART_BAUDRATE,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = SIM_UART_FLOWCTRL,
		.rx_flow_ctrl_thresh = 122,
	};

	ESP_RETURN_ON_ERROR(uart_param_config(SIM_UART_PORT, &uart_cfg),
			    SIM_LOG_TAG, "uart_config fail\r\n");
	ESP_RETURN_ON_ERROR(uart_set_pin(SIM_UART_PORT, SIM_UART_TX, SIM_UART_RX,
					 SIM_UART_RTS, SIM_UART_CTS),
			    SIM_LOG_TAG, "uart_set_pin fail\r\n");
	ESP_RETURN_ON_ERROR(uart_driver_install(SIM_UART_PORT, 2048, 2048, 10,
						NULL, 0),
			    SIM_LOG_TAG, "uart_driver_install fail\r\n");

	sim_mutex = xSemaphoreCreateMutex();
	if (sim_mutex == NULL) {
		ESP_LOGE(SIM_LOG_TAG, "Could not allocate mutex\n");
		return 1;
	}

	return 0;
}

int
sim_wait_for_rdy(void)
{
	size_t buff_len, read_len;
	int ret;
	TickType_t start_ticks;
	char buff[COMMAND_MAX_LENGTH];

	start_ticks = xTaskGetTickCount();
	buff_len = 0;
	ret = 1;

	do {
		read_len = uart_read_bytes(SIM_UART_PORT, &buff[buff_len],
					   sizeof(buff) - buff_len - 1, 0);
		buff_len += read_len;
		buff[buff_len] = '\0';

		if (strstr(buff, "RDY"))
			ret = 0;

		vTaskDelay(1);
	} while (ret == 1 && xTaskGetTickCount() - start_ticks <=
		 SIM_RDY_FLAG_TIMEOUT && buff_len < sizeof(buff));
	
	return ret;
}

int
sim_setup_module(void)
{
	int ret;

	ret = 0;

	ret = sim_send_command_await_response("OK", "ATE0\r");
	if (ret)
		ESP_LOGW(SIM_LOG_TAG, "ATE0 failed");
	vTaskDelay(1);


	if (sim_init_rtc())
		ESP_LOGW(SIM_LOG_TAG, "Failed to initialize RTC");

	ret += sim_mqtt_init(SIM_MQTT_CLIENT_NAME);
	if (ret)
		ESP_LOGW(SIM_LOG_TAG, "mqtt init failed");
	vTaskDelay(1);
	ret += sim_mqtt_connect(SIM_MQTT_BROKER_URL, 64800, 1, SIM_MQTT_USERNAME,
				SIM_MQTT_PASSWORD);

	return ret;
}

int
sim_send_command_await_response(char *expected_response, char *command, ...)
{
	char cmd[COMMAND_MAX_LENGTH];
	size_t cmd_len;
	va_list args;
	int ret;

	if (sim_take_mutex())
		return 1;

	va_start(args, command);
	cmd_len = vsnprintf(cmd, sizeof(cmd), command, args);
	va_end(args);
	if (cmd_len >= sizeof(cmd))
		cmd_len = sizeof(cmd) - 1;
	
	uart_flush(SIM_UART_PORT);

	/* send the command */
	uart_write_bytes(SIM_UART_PORT, cmd, cmd_len);

	/* await for the response */
	ret = sim_await_response(expected_response);

	sim_give_mutex();

	return ret;
}

int
sim_send_cmd_await_resp_unsafe(char *expected_response, char *command, ...)
{
	char cmd[COMMAND_MAX_LENGTH];
	size_t cmd_len;
	va_list args;

	/* make the command and send it */
	va_start(args, command);
	cmd_len = vsnprintf(cmd, sizeof(cmd), command, args);
	va_end(args);
	if (cmd_len >= sizeof(cmd))
		cmd_len = sizeof(cmd) - 1;
	
	uart_flush(SIM_UART_PORT);

	/* send the command */
	uart_write_bytes(SIM_UART_PORT, cmd, cmd_len);

	/* await for the response */
	return sim_await_response(expected_response);
}

int
sim_mqtt_init(char *client_name)
{
	int ret;

	if (sim_take_mutex())
		return 1;

	ret = 0;

	ret += sim_send_cmd_await_resp_unsafe("+CMQTTSTART: 0",
					       "AT+CMQTTSTART\r");
	vTaskDelay(1);
	ret += sim_send_cmd_await_resp_unsafe("OK",
					       "AT+CMQTTACCQ=0,\"%s\",0,4\r",
					       client_name);

	sim_give_mutex();

	return ret;
}

int
sim_mqtt_connect(char *address, uint16_t keepalive_time,
			 uint8_t clean_session, char *username, char *password)
{
	int ret;

	if (sim_take_mutex())
		return 1;

	if (username == NULL || *username == '\0') {
		ret = sim_send_cmd_await_resp_unsafe("+CMQTTCONNECT: 0,0",
				    "AT+CMQTTCONNECT=0,\"%s\",%" PRIu16 ",1\r",
				    address, keepalive_time);
	} else if (password == NULL || *password == '\0') {
		ret = sim_send_cmd_await_resp_unsafe("+CMQTTCONNECT: 0,0",
				 "AT+CMQTTCONNECT=0,\"%s\",%" PRIu16 ",1,\"%s\"\r",
				 address, keepalive_time, username);
	} else {
		ret = sim_send_cmd_await_resp_unsafe("+CMQTTCONNECT: 0,0",
			  "AT+CMQTTCONNECT=0,\"%s\",%" PRIu16 ",1,\"%s\",\"%s\"\r",
			  address, keepalive_time, username, password);
	}

	sim_give_mutex();

	return ret;
}

int
sim_mqtt_connect_frontend(void)
{
	return sim_mqtt_connect(SIM_MQTT_BROKER_URL, 64800, 1, SIM_MQTT_USERNAME,
				SIM_MQTT_PASSWORD);
}

int
sim_mqtt_publish(char *topic, uint8_t *message, size_t msg_size)
{
	int ret;

	if (sim_take_mutex())
		return 1;
	
	ret = 0;

	ret += sim_send_cmd_await_resp_unsafe(">", "AT+CMQTTTOPIC=0,%zu\r",
					       strlen(topic));
	vTaskDelay(1);
	ret += sim_send_cmd_await_resp_unsafe("OK", "%s", topic);
	vTaskDelay(1);
	ret += sim_send_cmd_await_resp_unsafe(">", "AT+CMQTTPAYLOAD=0,%zu\r",
					       msg_size);
	vTaskDelay(1);
	ret += sim_send_cmd_await_resp_unsafe("OK", "%s", message);
	vTaskDelay(1);
	ret += sim_send_cmd_await_resp_unsafe("OK", "AT+CMQTTPUB=0,0,120\r");

	sim_give_mutex();

	return ret;
}

/**
 * This function waits for the the newest response of the module and check to
 * see if it's the expected response
 * @param expected_response
 *
 * @returns 0 if the expected response was received or 1 otherwise
 */
static int
sim_await_response(char *expected_response)
{
	size_t buff_len, read_len;
	int ret;
	TickType_t start_ticks;
	char buff[COMMAND_MAX_LENGTH];

	start_ticks = xTaskGetTickCount();
	buff_len = 0;
	ret = 1;

	do {
		read_len = uart_read_bytes(SIM_UART_PORT, &buff[buff_len],
					   sizeof(buff) - buff_len - 1, 0);
		buff_len += read_len;
		buff[buff_len] = '\0';

		if (strstr(buff, expected_response))
			ret = 0;

		vTaskDelay(1);
	} while (ret == 1 && xTaskGetTickCount() - start_ticks <=
		 SIM_UNIVERSAL_TIMEOUT && buff_len < sizeof(buff));
	
	return ret;
}

static int
sim_init_rtc(void)
{
	const char cmd[] = "AT+CCLK?\r";
	size_t cmd_len = sizeof(cmd) - 1;
	char buff[COMMAND_MAX_LENGTH];
	size_t buff_len, read_len;
	uint8_t ret;
	TickType_t start_ticks;

	if (sim_take_mutex())
		return 1;

	/* send the command */
	uart_flush(SIM_UART_PORT);
	uart_write_bytes(SIM_UART_PORT, cmd, cmd_len);

	start_ticks = xTaskGetTickCount();
	buff_len = 0;
	ret = 1;

	do {
		read_len = uart_read_bytes(SIM_UART_PORT, &buff[buff_len],
					   sizeof(buff) - buff_len - 1, 0);
		buff_len += read_len;
		buff[buff_len] = '\0';

		if (strstr(buff, "OK"))
			ret = 0;

		vTaskDelay(1);
	} while (ret == 1 && xTaskGetTickCount() - start_ticks <=
		 SIM_UNIVERSAL_TIMEOUT && buff_len < sizeof(buff));

	puts(buff);

	if (ret == 0)
		sim_cclk_extract_info(buff);

	sim_give_mutex();

	return ret;
}

/**
 * TODO
 * internal callback function that decodes the date inside of resp and then sets
 * the rtc clock to that date (the rtc needs to be initialized already)
 */
static void
sim_cclk_extract_info(char *resp)
{
	struct rtc_calendar_t rtc_calendar;
	struct tm datetime = {0};
	time_t secs;
	int ret, timezone;
	char timezone_sign;

	/* get the date from the response into struct tm*/
	ret = sscanf(resp, "\r\n+CCLK: \"%d/%d/%d,%d:%d:%d%c%d\"", &datetime.tm_year,
		     &datetime.tm_mon, &datetime.tm_mday, &datetime.tm_hour,
		     &datetime.tm_min, &datetime.tm_sec, &timezone_sign,
		     &timezone);
	if (ret != 8) {
		ESP_LOGW(SIM_LOG_TAG,
			 "Could not get the current time from AT+CCLK?\n");
		return;
	}
	datetime.tm_isdst = -1;
	datetime.tm_mon--;
	datetime.tm_year += 100;

	/* year plausability: if the sim module doesn't have network, then its
	 * clock will be set in the year 2080
	 */
	if (datetime.tm_year >= 150)
		return;

	/* convert the struct tm from whatever local timezone to utc: convert
	 * datetime to seconds -> add the needed number of seconds (timezone is
	 * the number of quarter of hours) -> convert from seconds to datetime
	 */
	if (timezone_sign != '+' && timezone_sign != '-') {
		ESP_LOGW(SIM_LOG_TAG, "timezone sign unknown, continuing without converting the timezone");
		goto timezone_conversion_skip;
	}
	secs = mktime(&datetime);
	if (timezone_sign == '+')
		secs -= timezone * 900;
	else
		secs += timezone * 900;
	(void)gmtime_r(&secs, &datetime);

timezone_conversion_skip:
	/* convert from struct tm to struct rtc_calendar_t */
	rtc_tm_to_calendar(&datetime, &rtc_calendar);
	
	/* update the rtc calendar */
	if (rtc_set_calendar(rtc_calendar) != RTC_OK) {
		ESP_LOGW(SIM_LOG_TAG, "Could not update RTC\n");
		return;
	}
}

static int
sim_take_mutex(void)
{
	if (xSemaphoreTake(sim_mutex, SIM_UNIVERSAL_TIMEOUT) != pdTRUE)
		return 1;
	return 0;
}

static void
sim_give_mutex(void)
{
	xSemaphoreGive(sim_mutex);
}

/**
 * This function sends bytes to the uart peripheral and then waits for the
 * expected response
 */
static int
sim_send_data_await_resp_unsafe(char *expected_response, uint8_t *data,
				size_t data_size)
{
	uart_flush(SIM_UART_PORT);

	/* send the command */
	uart_write_bytes(SIM_UART_PORT, data, data_size);

	/* await for the response */
	return sim_await_response(expected_response);
}

