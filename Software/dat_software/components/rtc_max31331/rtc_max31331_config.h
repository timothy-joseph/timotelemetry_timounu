/*
 * File contatining the configuration for the rtc driver:
 * - pins
 * - i2c frequency
 * - i2c internal pullups
 * - registers
 * - update interval for sending the current timestamp on the can network
 */
#ifndef RTC_MAX31331_CONFIG_H
#define RTC_MAX31331_CONFIG_H

/*
 * The tag that will be used for the esp_log functions
 */
#define MICROSD_TAG "microsd"
#define RTC_LOG_TAG "max31331"

/*
 * The address of the rtc chip
 */
#define RTC_ADDR 0x68

/*
 * RTC registers
 */
#define RTC_PWR_MGMT 0x1A
#define RTC_TRICKLE_REG 0x1B
#define RTC_SECONDS_1_128 0x07

/*
 * i2c config
 */
#define RTC_I2C_PORT I2C_NUM_0
#define RTC_I2C_SDA GPIO_NUM_7
#define RTC_I2C_SCL GPIO_NUM_6
#define RTC_I2C_PULLUP GPIO_PULLUP_DISABLE
#define RTC_CLK_SPEED_HZ 400000

/*
 * universal timeout: for functions that require a tick timeout, this will be
 * be the value passed to it. The value is in freertos ticks
 */
#define RTC_UNIVERSAL_TIMEOUT 100

/*
 * the ms between sending updates to the dashboard's clock
 */
#define RTC_DASH_UPDATE_INTERVAL_MS 500
#define RTC_CAN_ID 0x10

#endif

