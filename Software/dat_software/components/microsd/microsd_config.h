/*
 * File containing the configuration for the microsd driver
 * - log tag
 * - pins for the data and clock lines
 * - default filename in the case that a file can't be opened with the name as
 * the current timestamp
 */
#ifndef MICROSD_CONFIG_H
#define MICROSD_CONFIG_H

/*
 * The tag that will be used for the esp_log functions
 */
#define MICROSD_TAG "microsd"

#define MICROSD_BASE_PATH "/sd"
/*
 * if reading from the rtc fails, then this will be then this will be the name
 * of the file that the DAT system will write to
 */
#define MICROSD_DEFAULT_FILENAME "log.txt"
#define MICROSD_FOPEN_MODE "ab"

/*
 * pins
 */
#define MICROSD_CLK GPIO_NUM_36
#define MICROSD_CMD GPIO_NUM_35
#define MICROSD_DAT0 GPIO_NUM_37
#define MICROSD_DAT1 GPIO_NUM_38
#define MICROSD_DAT2 GPIO_NUM_2
#define MICROSD_DAT3 GPIO_NUM_1

/*
 * other configs
 */
#define MICROSD_FORMAT_IF_MOUNT_FAILED 0
#define MICROSD_MAX_FILES 5

#endif

