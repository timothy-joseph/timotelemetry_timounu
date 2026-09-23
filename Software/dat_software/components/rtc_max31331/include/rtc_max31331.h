#ifndef RTC_MAX31331_H
#define RTC_MAX31331_H

#include <stdint.h>
#include <time.h>

#include <driver/gpio.h>

#include <freertos/FreeRTOS.h>

enum rtc_error_t {
	RTC_OK,
	RTC_FAIL,
	RTC_PARAM_ERROR,
	RTC_I2C_ERROR,
	RTC_MUTEX_ERROR
};

/**
 * struct that holds the exact values that are in the calendar registers of
 * the max31331 chip
 */
struct rtc_calendar_t {
	uint8_t seconds_1_128;
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
};

/**
 * function used to initialize i2c communication over these pins and with the
 * set master port
 *
 * @return 0 on success, 1 on failure
 */
int rtc_i2c_init(void);

/**
 * function used to enable the trickles charging feature
 *
 * @return 0 on success and 1 on failure
 */
int rtc_conf_trickle_charge(void);

/**
 * function used to set the current calendar date and time
 *
 * @return 0 on success and 1 on failure
 */
int rtc_set_calendar(struct rtc_calendar_t val);

/**
 * function used to get the current calendar date and time
 *
 * @return 0 on success and 1 on failure
 */
int rtc_get_calendar(struct rtc_calendar_t *ret);

/* functions used to convert between the max31331 calender spec and the tm spec
 */
void rtc_calendar_to_tm(struct rtc_calendar_t *val, struct tm *ret);
void rtc_tm_to_calendar(struct tm *val, struct rtc_calendar_t *ret);

/* functions used to convert between the unix epoch time and the max31331
 * calendar specification
 */
int32_t rtc_calendar_to_unix_time_sec(struct rtc_calendar_t *val);
void rtc_unix_sec_to_rtc_calendar(int32_t val, struct rtc_calendar_t *ret);

/* functions used to convert between the unix epoch time and the max31331
 * calendar specification, but instead of elapsed seconds since the epoch,
 * it's nanoseconds
 */
int64_t rtc_calendar_to_unix_time_us(struct rtc_calendar_t *val);
void rtc_unix_us_to_rtc_calendar(int64_t val, struct rtc_calendar_t *ret);

/**
 * Function used to start the periodic sending of can message to the dashboard
 *
 * @return 0 on success and 1 otherwise
 */
int rtc_start_periodic_can_msg(void);


#endif

