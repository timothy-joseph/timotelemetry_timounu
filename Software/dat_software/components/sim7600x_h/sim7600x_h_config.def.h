/*
 * File containing the configuration for the sim driver:
 * - thread stack sizes
 * - length of the commands queue
 * - rx and tx pins
 */
#ifndef SIM7600X_H_CONFIG_H
#define SIM7600X_H_CONFIG_H

/*
 * The tag that will be used for the esp_log functions
 */
#define SIM_LOG_TAG "sim_driver"

/*
 * uart pins and configuration
 */
#define SIM_UART_PORT UART_NUM_1
#define SIM_UART_BAUDRATE 115200
#define SIM_UART_RX GPIO_NUM_18
#define SIM_UART_TX GPIO_NUM_17
/* i found that hw control flow is unreliable; only UART_HW_FLOWCTRL_RTS works
 */
#define SIM_UART_RTS GPIO_NUM_19
#define SIM_UART_CTS GPIO_NUM_20
#define SIM_UART_FLOWCTRL UART_HW_FLOWCTRL_RTS

/*
 * universal timeout: for functions that require a tick timeout, this will be
 * be the value passed to it (unless they require portMAX_DELAY)
 *
 * this is also the timeout used when waiting for a response
 *
 * the value is in freertos ticks
 */
#define SIM_UNIVERSAL_TIMEOUT 300

/*
 * ready flag timeout: the timeout used by the sim_wait_for_rdy function
 */
#define SIM_RDY_FLAG_TIMEOUT 2000

/*
 * configuration for the threads and the commands queue
 */
#define SIM_RX_THREAD_STACK_SIZE 4096
#define SIM_ENQD_CMDS_THREAD_STACK_SIZE 4096
#define SIM_RX_THREAD_PRIORITY 2
#define SIM_ENQD_CMDS_THREAD_PRIORITY 2
#define SIM_CMDS_QUEUE_LENGTH 20

/*
 * mqtt configuration
 * NOTE: using hivemq for now because our broker isn't live yet
 */
#define SIM_MQTT_BROKER_URL "<++>"
#define SIM_MQTT_CLIENT_NAME "DAT"
#define SIM_MQTT_USERNAME "<++>"
#define SIM_MQTT_PASSWORD "<++>"

/*
 * GPS configuration (TODO)
 */
#define SIM_GPS_UPDATE_INTERVAL_MS 200
#define SIM_GPS_CAN_ID0 0x100
#define SIM_GPS_CAN_ID1 0x101

/**
 * after this amount of time, the periodic timer that updates the gps data will
 * be started
 */
#define SIM_GPSINFO_START_TIMEOUT_SEC 120

#endif

