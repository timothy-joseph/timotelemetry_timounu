/*
 * File containing the configuration for the vehicle status component
 * - can id's for the different messages
 */
#ifndef VEHICLE_STATUS_CONFIG_H
#define VEHICLE_STATUS_CONFIG_H

/*
 * The tag that will be used for the esp_log functions
 */
#define VS_LOG_TAG "vehicle_status"

/*
 * The ids of the expected messages
 */
#define VS_ECU0_ID 0x3e8
#define VS_ECU1_ID 0x3e9
#define VS_VDU0_ID 0x3ea
#define VS_VDU1_ID 0x3ec
#define VS_PMU0_ID 0x3ed
#define VS_PMU1_ID 0x3ee
#define VS_PMU2_ID 0x3ef
#define VS_DASH_ID 0x384
#define VS_SCS_ID  0x3f0
#define VS_DAT0_ID 0x10
#define VS_DAT1_ID 0x11
#define VS_DAT2_ID 0x12
#define VS_DAT3_ID 0x13
#define VS_RACIRE0_ID 0x3f4
#define VS_RACIRE1_ID 0x3f5
#define VS_RACIRE2_ID 0x3f6
#define VS_RACIRE3_ID 0x3f7
#define VS_RACIRE4_ID 0x3f8
#define VS_RACIRE5_ID 0x3f9

/*
 * timeout passed to xSemaphoreTake (freertos ticks)
 */
#define VS_MUTEX_TIMEOUT 50


#define VS_MQTT_TOPIC "test_tj_123"
#define VS_MQTT_UPDATE_INTERVAL_MS 50

#define VS_THREAD_STACK_SIZE 4096
#define VS_THREAD_PRIORITY 2

#endif

