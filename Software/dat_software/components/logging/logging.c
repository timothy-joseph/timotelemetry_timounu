#include <stdio.h>
#include <stdint.h>

#include <driver/twai.h>
#include <esp_log.h>

#include <sim7600x_h.h>
#include <microsd.h>
#include <rtc_max31331.h>
#include <lora_driver.h>

#include "logging.h"

#include "logging_config.h"

static uint8_t logging_build_packet(uint8_t *packet, uint32_t timestamp,
				    const twai_message_t *msg);

static int64_t start_timestamp_us;

void
logging_init_component(void)
{
	struct rtc_calendar_t start_calendar;
	int32_t start_timestamp_sec;

	rtc_get_calendar(&start_calendar);
	start_timestamp_sec = rtc_calendar_to_unix_time_sec(&start_calendar);
	/* log start_timestamp_sec using the normal packet format using a madeup
	 * id
	 */
	start_timestamp_us = rtc_calendar_to_unix_time_us(&start_calendar);
}

void
logging_log_can_msg_to_sd(const twai_message_t *msg)
{
	struct rtc_calendar_t current_calendar;
	int64_t current_timestamp_us;
	uint32_t timestamp_to_send_us;
	uint8_t packet[15], packet_size;

	/* get timestamp */
	rtc_get_calendar(&current_calendar);
	current_timestamp_us =
		rtc_calendar_to_unix_time_us(&current_calendar);
	timestamp_to_send_us = current_timestamp_us - start_timestamp_us;
	
	/* build packet */
	packet_size = logging_build_packet(packet, timestamp_to_send_us, msg);

	/* log packet using the 3 methods */
	microsd_write_to_log_file(packet, packet_size);

#if 0
	/* TODO: remove after testing */
	uint8_t i;
	for (i = 0; i < packet_size; i++)
		printf("%02hhx ", packet[i]);
	printf("\n");
#endif
}

uint32_t
logging_get_timestamp_us(void)
{
	struct rtc_calendar_t current_calendar;
	int64_t current_timestamp_us;
	uint32_t timestamp_to_send_us;

	rtc_get_calendar(&current_calendar);
	current_timestamp_us =
		rtc_calendar_to_unix_time_us(&current_calendar);
	timestamp_to_send_us = current_timestamp_us - start_timestamp_us;

	return timestamp_to_send_us;
}

/* writes to buff 4 bytes from the uint32_t data in big-endian format */
void
logging_uint32_t_to_big_endian(uint32_t data, uint8_t *buff)
{
	buff[0] = (uint8_t)((data >> 24) & 0xff);
	buff[1] = (uint8_t)((data >> 16) & 0xff);
	buff[2] = (uint8_t)((data >> 8) & 0xff);
	buff[3] = (uint8_t)(data & 0xff);
}

void
logging_uint16_t_to_big_endian(uint16_t data, uint8_t *buff)
{
	buff[0] = (uint8_t)((data >> 8) & 0xff);
	buff[1] = (uint8_t)(data & 0xff);
}


static uint8_t
logging_build_packet(uint8_t *packet, uint32_t timestamp,
		     const twai_message_t *msg)
{
	uint8_t i;

	logging_uint32_t_to_big_endian(timestamp, packet);
	logging_uint16_t_to_big_endian((uint16_t)msg->identifier, &packet[4]);
	packet[6] = msg->data_length_code;

	for (i = 0; i < msg->data_length_code; i++)
		packet[i + 7] = msg->data[i];

	return msg->data_length_code + 7;
}

