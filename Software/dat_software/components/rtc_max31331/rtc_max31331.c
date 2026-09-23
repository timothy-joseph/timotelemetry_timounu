/*
 * File structure:
 * 1. extern functions
 *	- init i2c driver
 *	- setup rtc chip
 *	- read and get rtc calendar
 *	- convert between rtc calendar and unix timespec, and vice-versa
 * 2. static functions
 *	- mutex functions
 *	- reading and writing to registers
 *	- functions that help with converting between rtc calendar and unix
 *	  timespec and vice-versa
 */
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <esp_log.h>
#include <esp_check.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include <driver/i2c.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <can.h>

#include "rtc_max31331.h"

#include "rtc_max31331_config.h"

static inline uint8_t rtc_bcd_to_bin(uint8_t x);
static inline uint8_t rtc_bin_to_bcd(uint8_t x);
static int rtc_take_mutex(void);
static void rtc_give_mutex(void);
static int rtc_write_to_register_no_mutex(uint8_t reg, uint8_t val);
static int rtc_write_to_register(uint8_t reg, uint8_t val);
static int rtc_write_to_n_registers(uint8_t reg, uint8_t *buff, size_t n);
static int rtc_read_register_no_mutex(uint8_t reg, uint8_t *ret);
static int rtc_read_register(uint8_t reg, uint8_t *ret);
static int rtc_read_n_registers(uint8_t start_reg, uint8_t *buff, size_t n);
static int rtc_get_yday(struct tm *datetime);
static int rtc_is_leap_year(int year);
static void rtc_dashboard_update(void);

/* mutex used so that 2 threads can't access the rtc's i2c at the same time */
static SemaphoreHandle_t rtc_mutex;

int
rtc_i2c_init(void)
{
	i2c_config_t i2c_conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = RTC_I2C_SDA,
		.sda_pullup_en = RTC_I2C_PULLUP,
		.scl_io_num = RTC_I2C_SCL,
		.scl_pullup_en = RTC_I2C_PULLUP,
		/* max 400000 */
		.master.clk_speed = RTC_CLK_SPEED_HZ,
		.clk_flags = 0,
	};

	rtc_mutex = xSemaphoreCreateMutex();
	if (rtc_mutex == NULL) {
		ESP_LOGE(RTC_LOG_TAG, "Could not allocate mutex\n");
		return RTC_MUTEX_ERROR;
	}

	ESP_RETURN_ON_ERROR(i2c_param_config(RTC_I2C_PORT, &i2c_conf),
			    RTC_LOG_TAG, "i2c_param_config fail\n");
	ESP_RETURN_ON_ERROR(i2c_driver_install(RTC_I2C_PORT, i2c_conf.mode,
						0, 0, 0),
			    RTC_LOG_TAG, "i2c_driver_install fail\n");

	return rtc_conf_trickle_charge();
}

int
rtc_conf_trickle_charge(void)
{
	return rtc_write_to_register(RTC_TRICKLE_REG, (0x7 << 1) | 1);
}

int
rtc_set_calendar(struct rtc_calendar_t val)
{
	return rtc_write_to_n_registers(RTC_SECONDS_1_128, (uint8_t *)&val,
					sizeof(val));
}

int
rtc_get_calendar(struct rtc_calendar_t *ret)
{
	return rtc_read_n_registers(RTC_SECONDS_1_128, (uint8_t *)ret,
				    sizeof(*ret));
}

void
rtc_calendar_to_tm(struct rtc_calendar_t *val, struct tm *ret)
{
	/* century bit is not taken into account because for this application,
	 * everything is assumed to be in the 21st century
	 */
	ret->tm_year = 100 + rtc_bcd_to_bin(val->year);
	ret->tm_mon = rtc_bcd_to_bin(val->month) - 1;
	ret->tm_mday = rtc_bcd_to_bin(val->date);
	ret->tm_wday = rtc_bcd_to_bin(val->day) - 1;
	ret->tm_hour = rtc_bcd_to_bin(val->hours & 0x1f);
	/* 12 hour format with pm indicator*/
	if ((val->hours & 0x60) == 0x60)
		ret->tm_hour += 12;
	ret->tm_min = rtc_bcd_to_bin(val->minutes);
	ret->tm_sec = rtc_bcd_to_bin(val->seconds);
	ret->tm_yday = rtc_get_yday(ret);
	ret->tm_isdst = -1;
}

void
rtc_tm_to_calendar(struct tm *val, struct rtc_calendar_t *ret)
{
#if 0
	printf("datetime = %d_%d_%d-%d:%d:%d\n", val->tm_year, val->tm_mon,
		val->tm_mday, val->tm_hour, val->tm_min, val->tm_sec);
#endif
	ret->seconds_1_128 = 0;
	ret->seconds = rtc_bin_to_bcd(val->tm_sec);
	ret->minutes = rtc_bin_to_bcd(val->tm_min);
	ret->hours = rtc_bin_to_bcd(val->tm_hour);
	ret->day = rtc_bin_to_bcd(val->tm_wday + 1);
	ret->date = rtc_bin_to_bcd(val->tm_mday);
	ret->month = rtc_bin_to_bcd(val->tm_mon + 1);
	/* i assume we won't need the 22nd century functionality of the
	 * max31331 chip
	 */
	ret->year = rtc_bin_to_bcd(val->tm_year - 100);
}

int32_t
rtc_calendar_to_unix_time_sec(struct rtc_calendar_t *val)
{
	struct tm datetime;

	if (!val)
		return RTC_OK;
	
	rtc_calendar_to_tm(val, &datetime);

	return mktime(&datetime);
}

void
rtc_unix_sec_to_rtc_calendar(int32_t val, struct rtc_calendar_t *ret)
{
	struct tm datetime;
	time_t val_tmp;

	if (!ret)
		return;
	
	/* it's assumed that time_t is an integer type */
	val_tmp = val;

	(void)gmtime_r(&val_tmp, &datetime);
	rtc_tm_to_calendar(&datetime, ret);
}

int64_t
rtc_calendar_to_unix_time_us(struct rtc_calendar_t *val)
{
	int32_t usec;

	if (!val)
		return RTC_OK;

	usec = (1000000 * val->seconds_1_128) >> 7;

	return (int64_t)rtc_calendar_to_unix_time_sec(val) * 1000000 + usec;
}

void
rtc_unix_us_to_rtc_calendar(int64_t val, struct rtc_calendar_t *ret)
{
	if (!ret)
		return;
	
	rtc_unix_sec_to_rtc_calendar(val / 1000000, ret);
	ret->seconds_1_128 = ((val % 1000000) << 7) / 1000000;
}

int
rtc_start_periodic_can_msg(void)
{
	esp_timer_handle_t dash_update_timer;
	esp_timer_create_args_t dash_update_timer_conf = {
		.callback = rtc_dashboard_update,
		.arg = NULL,
		.dispatch_method = ESP_TIMER_TASK,
		.name = NULL,
		.skip_unhandled_events = 0
	};

	if (esp_timer_create(&dash_update_timer_conf, &dash_update_timer))
		return 1;
	
	return esp_timer_start_periodic(dash_update_timer,
					RTC_DASH_UPDATE_INTERVAL_MS);
}

static inline uint8_t
rtc_bcd_to_bin(uint8_t x)
{
	return ((x >> 4) * 10) + (x & 0xf);
}

static inline uint8_t
rtc_bin_to_bcd(uint8_t x)
{
	return ((x / 10) << 4) + (x % 10);
}

static int
rtc_take_mutex(void)
{
	if (xSemaphoreTake(rtc_mutex, RTC_UNIVERSAL_TIMEOUT) != pdTRUE)
		return RTC_MUTEX_ERROR;
	return RTC_OK;
}

static void
rtc_give_mutex(void)
{
	xSemaphoreGive(rtc_mutex);
}

/**
 * function used to write to a single register in the rtc chip without first
 * acquiring the mutex
 */
static int
rtc_write_to_register_no_mutex(uint8_t reg, uint8_t val)
{
	uint8_t to_send[2];

	to_send[0] = reg;
	to_send[1] = val;

	return i2c_master_write_to_device(RTC_I2C_PORT, RTC_ADDR, to_send,
					  sizeof(to_send),
					  RTC_UNIVERSAL_TIMEOUT);
}

/**
 * wrapper for rtc_write_to_register_no_mutex that acquires the mutex before
 * calling it. the function gives the mutex back
 */
static int
rtc_write_to_register(uint8_t reg, uint8_t val)
{

	int ret;

	if (rtc_take_mutex())
		return RTC_MUTEX_ERROR;
	
	ret = rtc_write_to_register_no_mutex(reg, val);

	rtc_give_mutex();

	return ret;
}


/* function that acquires the i2c mutex and then calls
 * rtc_write_to_register_no_mutex n times
 */
static int
rtc_write_to_n_registers(uint8_t start_reg, uint8_t *buff, size_t n)
{
	int ret;
	size_t i;

	if (rtc_take_mutex())
		return RTC_MUTEX_ERROR;
	
	for (i = 0; i < n; i++) {
		ret = rtc_write_to_register_no_mutex(start_reg + i, buff[i]);
		if (ret) {
			rtc_give_mutex();
			return ret;
		}
	}

	rtc_give_mutex();

	return RTC_OK;
}

/**
 * function used to read a single register in the rtc chip without first
 * acquiring the mutex
 */
static int
rtc_read_register_no_mutex(uint8_t reg, uint8_t *ret)
{
	uint8_t to_send[1];

	to_send[0] = reg;

	return i2c_master_write_read_device(RTC_I2C_PORT, RTC_ADDR,
					    to_send, sizeof(to_send),
					    ret, 1,
					    RTC_UNIVERSAL_TIMEOUT);
}

/**
 * wrapper for rtc_read_register_no_mutex that acquires the mutex before
 * calling it. the function gives the mutex back
 */
static int
rtc_read_register(uint8_t reg, uint8_t *ret)
{
	int rret;

	if (rtc_take_mutex())
		return RTC_MUTEX_ERROR;
	
	rret = rtc_read_register_no_mutex(reg, ret);

	rtc_give_mutex();

	return rret;
}

/* function that acquires the i2c mutex and then calls
 * rtc_read_to_register_no_mutex n times
 */
static int
rtc_read_n_registers(uint8_t start_reg, uint8_t *buff, size_t n)
{
	int ret;
	size_t i;

	if (rtc_take_mutex())
		return RTC_MUTEX_ERROR;
	
	for (i = 0; i < n; i++) {
		ret = rtc_read_register_no_mutex(start_reg + i, &buff[i]);
		if (ret) {
			rtc_give_mutex();
			return ret;
		}
	}

	rtc_give_mutex();

	return RTC_OK;
}

static int
rtc_get_yday(struct tm *datetime)
{
	int ret;

	const int days_sum[] = {
		0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
	};

	ret = days_sum[datetime->tm_mon];
	if (datetime->tm_mon > 1 && rtc_is_leap_year(datetime->tm_year))
		ret++;

	return ret;
}

static int
rtc_is_leap_year(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

/**
 * function used to give time updates to the dashboard
 */
static void
rtc_dashboard_update(void)
{
	struct rtc_calendar_t now;
	twai_message_t msg = {0};

	/* get the needed data */
	rtc_get_calendar(&now);

	/* build the message */
	msg.identifier = RTC_CAN_ID;
	msg.extd = 0;
	msg.data_length_code = 3;
	msg.data[0] = rtc_bcd_to_bin(now.hours & 0x1f);
	if ((now.hours & 0x60) == 0x60)
		msg.data[0] += 12;
	msg.data[1] = rtc_bcd_to_bin(now.minutes);
	msg.data[2] = rtc_bcd_to_bin(now.seconds);

	can_send_message(&msg);
}

