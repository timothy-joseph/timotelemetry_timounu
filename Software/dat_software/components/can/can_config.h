/*
 * File containing the configuration for the can driver
 * - pins
 * - stack size for the receive thread
 */
#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

/*
 * The tag that will be used for the esp_log functions
 */
#define CAN_TAG "can"

/*
 * pins
 */
#define CAN_TX_PIN GPIO_NUM_14
#define CAN_RX_PIN GPIO_NUM_21

/*
 * size of the thread that receives all can messages
 */
#define CAN_RX_THREAD_STACK_SIZE 4096

/*
 * priority of the thread that receives all can messages
 */
#define CAN_RX_THREAD_PRIORITY (tskIDLE_PRIORITY + 1)

/*
 * universal timeout: for functions that require a tick timeout, this will be
 * be the value passed to it (unless they require portMAX_DELAY)
 *
 * The value is in freertos ticks
 */
#define CAN_UNIVERSAL_TIMEOUT 100


#endif

