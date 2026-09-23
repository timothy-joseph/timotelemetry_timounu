/*
 * Cicu Timotei-Iosif (timothy_joseph) pentru UPBDrive
 * reference: https://github.com/andresoliva/Grove-Wio-E5/
 * reference: https://wiki.seeedstudio.com/Grove_LoRa_E5_New_Version/
 * reference: https://wiki.seeedstudio.com/LoRa-E5_STM32WLE5JC_Module/
 *
 *
 * TODO:
 *	1. check if the string manipulations are safe
 *	2. check configuration options
 *	3. do configurations options for receiver
 *
 */
#ifndef LORA_DRIVER_H
#define LORA_DRIVER_H

#include <stdint.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#include <freertos/queue.h>
#include <freertos/semphr.h>

/* change this to switch between TEST mode and NORMAL mode (not implemented
 * yet; see at command specification)
 */
#define AT_TEST_MODE
#if 0
#define AT_NORMAL_MODE
#endif

/* The maximum command length is specified in the AT command manual to be 528 */
#define LORA_COMMAND_LENGTH 529
#define LORA_RESPONSE_LENGTH 529

/* TODO: find a good arbitrary number for this */
#define LORA_MESSAGE_LENGTH 529

/**
 * Macro used to declare a struct lora_dev_t with the name as the parameter
 * NAME. The variable will be 0 initialized
 *
 * @param NAME The name of the variable
 */
#define DECLARE_LORA_DEV_T(NAME)           \
	struct lora_dev_t NAME = {         \
		.uart_num = 0,             \
		.payload_queue = NULL,     \
		.uart_event_queue = NULL,  \
		.received_messages = NULL, \
		.universal_timeout = 100,  \
		.last_response = "",       \
		.received_response = 0,    \
		.resp_mutex = 0,           \
	};

struct lora_dev_t {
	uart_port_t uart_num;
	QueueHandle_t payload_queue;
	QueueHandle_t uart_event_queue;
	QueueHandle_t received_messages;
	TickType_t universal_timeout;
	char last_response[LORA_RESPONSE_LENGTH];
	uint8_t received_response;
	SemaphoreHandle_t resp_mutex;
	/* Optional if needed: add received_error_queue */
};

struct lora_payload_t {
	char command[LORA_COMMAND_LENGTH];
	char expected_response[LORA_RESPONSE_LENGTH];
};

struct lora_message_t {
	uint8_t data[LORA_MESSAGE_LENGTH];
	size_t size;
};

/**
 * Function used to initialize the uart port to which the Wio-E5 module is
 * connected to. This thread also creates one queue (used for uart events) and
 * a thread. The length of the event queue is fixed at event queue is fixed at
 * TODO and the stack size is TODO
 *
 * @param handle [out] A pointer to a struct lora_dev_t, the variable this
 *		 points to must not go out of scope.
 * @param uart_num
 * @param tx_pin
 * @param rx_pin
 * @param rx_thread_prio The priority of the thread that receives uart events
 * @param universal_timeout This timeout will be passed to all functions that
 *			    that have a timeout
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
int lora_init_uart(struct lora_dev_t *handle, uart_port_t uart_num,
		   gpio_num_t tx_pin, gpio_num_t rx_pin, uint8_t rx_thread_prio,
		   TickType_t universal_timeout);

/**
 * Function used to initialize the queue and thread that will be used to send
 * commands to the lora module like this: the thread waits for a new command to
 * be put unto the queue, it sends the command over uart and waits for the
 * expected response. The created thread has a stack size of TODO
 *
 * @param handle [out] A pointer to a strcut lora_dev_t, the variable this
 *		 points to must not go out of scope because it will be given as
 *		 a parameter to the thread
 * @param queue_len
 * @param thread_priority
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
int lora_init_as_sender(struct lora_dev_t *handle, size_t queue_len,
			int thread_priority);

/**
 * Function used to send the AT+TEST=RXLRPKT for continuous receiving of
 * packages. This function also allocates the message queue that can be used as
 * an event queue for your own threads
 *
 * @param handle A pointer to a struct lora_dev_t
 * @param msg_queue_len The amount of messages the queue can hold at one time
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
int lora_init_as_receiver(struct lora_dev_t *handle, size_t msg_queue_len);

/**
 * Function used to put a command unto the queue that the thread for sending
 * checks.
 *
 * @param handle A initialized lora device
 * @param expected_response The ok message that is expected (null terminated
 *			    string)
 * @param command A formated string representing the command in printf format
 *		  (null terminated string)
 * @param ... The format information
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
int lora_enqueue_command(struct lora_dev_t *handle, char *expected_response,
			 char *command, ...);

/**
 * Function used to put enqueue a command in order to send a hex message.
 *
 * @param handle A initialized lora device
 * @param message The message to send (in bytes, not in hex format as the
 *		  datasheet specifies, the function translates the message
 *		  into hex format)
 *
 * @return ESP_OK on success
 *	   ESP_FAIL otherwise
 */
int lora_enqueue_hexmsg(struct lora_dev_t *handle, uint8_t *message,
			size_t message_size);

#if 0
#undef LORA_COMMAND_LENGTH
#endif

#endif /* #ifndef LORA_DRIVER_H */

