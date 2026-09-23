/* Written by timothy-joseph 2024
 * Only the functions that i need of the sim module are implemented
 */
/**
 * Usage:
 * 1. Initialize the uart interface (see esp32 documentation: 115200mbps)
 * 2. Set the control flow mode of the sim device according to the mode that
 *    you initialized the uart interface to
 * 3. Call
 */
#ifndef SIM7600X_H_H
#define SIM7600X_H_H

#include <time.h>

#define COMMAND_MAX_LENGTH 200

struct sim_gps_info {
	uint32_t lat; /*< fixed point, 6 digits after . */
	char lat_indicator;
	uint32_t log; /*< fixed point, 6 digits after . */
	char log_indicator;
	struct tm date_time;
	int16_t altitude; /*< fixed point, 1 digit after . */
	int16_t speed_knots; /*< fixed point, 1 digit after . */
	uint16_t course_degrees; /*< fixed point, 1 digit after . */
};

/*
 * Setup functions
 */

/**
 * This function initializes: the uart driver, the commands queue thead and
 * the uart events thread
 *
 * @return 0 on success and 1 otherwise
 */
int sim_init_uart(void);

/**
 * This function blocks execution until the RDY flag is received from the sim
 * module. A timeout is configurable from inside of the config file of this
 * component.
 *
 * @return 0 if the RDY flag was received before the timeout and 1 otherwise
 *
 * TODO: the structure of this function is the same as sim_await_response, but
 * it has a different timeout: consider making a function that has the
 * expected_response and the timeout as parameters
 */
int sim_wait_for_rdy(void);

/**
 * Function used to send a few initializing commands to the sim module and also
 * start the periodic timer for updating the gps location.
 *
 * As part of the initializing commands, the mqtt is started and is connected
 * to the server
 *
 * The commands sent are
 * ATE0 -> turn off command echo
 * AT+CCLK -> get the current date and time from the sim module inorder to
 *	      update the rtc of the max31331 chip
 * AT+CGPS=1 -> start the gps
 * (mqtt commands ommited from this comment)
 */
int sim_setup_module(void);

/*
 * Command functions
 */

/**
 * This function sends a command and waits a maximum of universal_timeout
 * amount of ticks for the response. This command doesn't check if a new
 * command can be sent. The uart peripheral is protected with a mutex
 *
 * @param expected_response
 * @param command formatted string
 * @param ... formatted string arguments
 *
 * @return 0 if the command returned the expected response and 1 otherwise
 */
int sim_send_command_await_response(char *expected_response, char *command,
				    ...);

/**
 * Same as sim_send_command_await_response, but without a mutex
 *
 * @param expected_response
 * @param command formatted string
 * @param ... formatted string arguments
 *
 * @return 0 if the command returned the expected response and 1 otherwise
 */
int sim_send_cmd_await_resp_unsafe(char *expected_response, char *command, ...);

/*
 * mqtt command functions
 */

/**
 * This command is used to execute the AT+CMQTTSTART and AT+CMQTTACCQ commands
 */
int sim_mqtt_init(char *client_name);

/**
 * This command is used to execute the AT+CMQTTCONNECT command
 */
int sim_mqtt_connect(char *address, uint16_t keepalive_time,
			     uint8_t clean_session, char *username,
			     char *password);

/**
 * function used outside of the sim component to connect to the server. I
 * sending errors might be because of a connection error
 */
int sim_mqtt_connect_frontend(void);

/**
 * This command is used to execute the
 * AT+CMQTTTOPIC, AT+CMQTTMSG, and AT+CMQTTPUB commands one after the other
 */
int sim_mqtt_publish(char *topic, uint8_t *message, size_t msg_size);

#endif

