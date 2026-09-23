#ifndef CAN_H
#define CAN_H

#include <driver/twai.h>

#include <freertos/FreeRTOS.h>

/**
 * function used to setup the can component. this function also starts the
 * receive thread
 *
 * @return 0 on success and 1 on failure
 */
int can_setup(void);

/**
 * function used to send a message on the can. the can network is secured with
 * a mutex
 *
 * @return 0 on success and 1 otherwise
 */
int can_send_message(twai_message_t *msg);
TaskHandle_t can_create_receive_thread(void);

#endif

