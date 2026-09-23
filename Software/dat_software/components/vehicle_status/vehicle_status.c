#include <stdio.h>

#include <esp_log.h>
#include <esp_check.h>
#include <esp_timer.h>
#include "driver/twai.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <sim7600x_h.h>
#include <logging.h>

#include <vehicle_status.h>

#include "vehicle_status_config.h"

static void vs_logger_thread(void *args);
static void vs_mqtt_log_periodic_cb(void);
static int vs_take_mutex(void);
static void vs_give_mutex(void);
static int vs_build_mqtt_packet(uint8_t *packet);
static size_t vs_packet_to_hexstr(uint8_t *packet, size_t packet_size,
				  char *hexstr, size_t hexstr_size);
static char vs_get_hex_digit(uint8_t digit);
static void vs_update_ecu0(twai_message_t *msg);
static void vs_update_ecu1(twai_message_t *msg);
static void vs_update_vdu0(twai_message_t *msg);
static void vs_update_vdu1(twai_message_t *msg);
static void vs_update_pmu0(twai_message_t *msg);
static void vs_update_pmu1(twai_message_t *msg);
static void vs_update_pmu2(twai_message_t *msg);
static void vs_update_dash(twai_message_t *msg);
static void vs_update_scs(twai_message_t *msg);
static void vs_update_dat0(twai_message_t *msg);
static void vs_update_dat1(twai_message_t *msg);
static void vs_update_dat2(twai_message_t *msg);
static void vs_update_dat3(twai_message_t *msg);
static void vs_update_racire0(twai_message_t *msg);
static void vs_update_racire1(twai_message_t *msg);
static void vs_update_racire2(twai_message_t *msg);
static void vs_update_racire3(twai_message_t *msg);
static void vs_update_racire4(twai_message_t *msg);
static void vs_update_racire5(twai_message_t *msg);
static inline size_t vs_min(size_t x, size_t y);

struct vehicle_status_t vs_global;
static SemaphoreHandle_t vs_mutex;

int
vs_init(void)
{
	/* create the mutex */
	BaseType_t thread_return;
	TaskHandle_t thread_handle;

	vs_mutex = xSemaphoreCreateMutex();
	if (!vs_mutex) {
		ESP_LOGE(VS_LOG_TAG, "Failed to create the mutex\n");
		return 1;
	}

	/* create the thread */
	thread_return = xTaskCreatePinnedToCore(vs_logger_thread,
						"vs_logger_thread",
						VS_THREAD_STACK_SIZE, NULL,
						VS_THREAD_PRIORITY,
						&thread_handle,
						tskNO_AFFINITY);
	return thread_return != pdPASS;
}

int
vs_update_from_can_msg(twai_message_t *msg)
{
	int ret;

	if (vs_take_mutex())
		return 1;

	ret = 0;

	switch (msg->identifier) {
	case VS_ECU0_ID:
		vs_update_ecu0(msg);
		break;
	case VS_ECU1_ID:
		vs_update_ecu1(msg);
		break;
	case VS_VDU0_ID:
		vs_update_vdu0(msg);
		break;
	case VS_VDU1_ID:
		vs_update_vdu1(msg);
		break;
	case VS_PMU0_ID:
		vs_update_pmu0(msg);
		break;
	case VS_PMU1_ID:
		vs_update_pmu1(msg);
		break;
	case VS_PMU2_ID:
		vs_update_pmu2(msg);
		break;
	case VS_DASH_ID:
		vs_update_dash(msg);
		break;
	case VS_SCS_ID:
		vs_update_scs(msg);
		break;
	case VS_DAT0_ID:
		vs_update_dat0(msg);
		break;
	case VS_DAT1_ID:
		vs_update_dat1(msg);
		break;
	case VS_DAT2_ID:
		vs_update_dat2(msg);
		break;
	case VS_DAT3_ID:
		vs_update_dat3(msg);
		break;
	case VS_RACIRE0_ID:
		vs_update_racire0(msg);
		break;
	case VS_RACIRE1_ID:
		vs_update_racire1(msg);
		break;
	case VS_RACIRE2_ID:
		vs_update_racire2(msg);
		break;
	case VS_RACIRE3_ID:
		vs_update_racire3(msg);
		break;
	case VS_RACIRE4_ID:
		vs_update_racire4(msg);
		break;
	case VS_RACIRE5_ID:
		vs_update_racire5(msg);
		break;
	default:
#if 0
		ESP_LOGW(VS_LOG_TAG, "Unknown can id\n");
		printf("%lx\n", msg->identifier);
#endif
		ret = 1;
	}

	vs_give_mutex();

	return ret;
}

static void
vs_logger_thread(void *args)
{
	(void)args;

	while (1) {
		vTaskDelay(VS_MQTT_UPDATE_INTERVAL_MS / portTICK_PERIOD_MS);
		vs_mqtt_log_periodic_cb();
	}
}

/**
 * periodic functions that sends vehicle status updates to the mqtt server
 */
static void
vs_mqtt_log_periodic_cb(void)
{
	uint8_t packet[102];
	char hexstr[250];
	size_t hexstr_len;
	int ret;

	vs_build_mqtt_packet(packet);
	hexstr_len = vs_packet_to_hexstr(packet, sizeof(packet), hexstr,
					 sizeof(hexstr));
	printf("data: %s\n", hexstr);
	ret = sim_mqtt_publish(VS_MQTT_TOPIC, (uint8_t *)hexstr,
			       hexstr_len);
	if (ret) {
		ESP_LOGW(VS_LOG_TAG, "Could not send the data using mqtt\n");
		ESP_LOGW(VS_LOG_TAG, "Attempting to reconnect\n");
		sim_mqtt_connect_frontend();
	}
}

/**
 * function used to build the mqtt packet like this: 4 bytes representing a
 * timestamp (microseconds), then vehicle_status_t. All non-byte data is in big
 * endian format
 */
static int
vs_build_mqtt_packet(uint8_t *packet)
{
	uint64_t timestamp;

	if (vs_take_mutex())
		return 1;

	timestamp = logging_get_timestamp_us();
	logging_uint32_t_to_big_endian(timestamp, &packet[0]);

	packet[4] = vs_global.rpm;
	packet[5] = vs_global.curr_gear;
	packet[6] = vs_global.tps;
	packet[7] = vs_global.oil_pres;
	packet[8] = vs_global.coolant_temp;
	packet[9] = vs_global.fuel_pres;
	packet[10] = vs_global.lambda;
	packet[11] = vs_global.iat;

	/* ECU1 */
	packet[12] = vs_global.egt1;
	packet[13] = vs_global.egt2;
	packet[14] = vs_global.egt3;
	packet[15] = vs_global.egt4;
	packet[16] = vs_global.vehicle_speed;
	packet[17] = vs_global.map;
	packet[18] = vs_global.bps;
	packet[19] = vs_global.oil_temp;

	/* VDU0 */
	packet[20] = vs_global.susLF;
	packet[21] = vs_global.susRF;
	packet[22] = vs_global.susLR;
	packet[23] = vs_global.susRR;
#if 0
	/* Achizitii date racire */
	logging_uint16_t_to_big_endian(vs_global.accel, &packet[24]);
	logging_uint16_t_to_big_endian(vs_global.accel, &packet[26]);
#endif

	/* VDU1 */
	packet[28] = vs_global.tyre_presLF;
	packet[29] = vs_global.tyre_presRF;
	packet[30] = vs_global.tyre_presLR;
	packet[31] = vs_global.tyre_presRR;

	/* PMU0 */
	packet[32] = vs_global.lv_bat_volts;
	packet[33] = vs_global.in_voltage;
	packet[34] = vs_global.in_cur_bat;
	packet[35] = vs_global.in_cur_alt;

	/* PMU1 */
	packet[36] = vs_global.output_current_0;
	packet[37] = vs_global.output_current_1;
	packet[38] = vs_global.output_current_2;
	packet[39] = vs_global.output_current_3;
	packet[40] = vs_global.output_current_4;
	packet[41] = vs_global.output_current_5;
	packet[42] = vs_global.output_current_6;
	packet[43] = vs_global.output_current_7;

	/* PMU2 */
	packet[44] = vs_global.output_voltage_0;
	packet[45] = vs_global.output_voltage_1;
	packet[46] = vs_global.output_voltage_2;
	packet[47] = vs_global.output_voltage_3;
	packet[48] = vs_global.output_voltage_4;
	packet[49] = vs_global.output_voltage_5;
	packet[50] = vs_global.output_voltage_6;
	packet[51] = vs_global.output_voltage_7;

	/* DASH */
	packet[52] = vs_global.dash_selector;

	/* SCS */
	packet[53] = vs_global.scs;

	/* racire */
	logging_uint32_t_to_big_endian(vs_global.water_in_stanga, &packet[54]);
	logging_uint32_t_to_big_endian(vs_global.water_in_dreapta, &packet[58]);
	logging_uint32_t_to_big_endian(vs_global.water_out_stanga, &packet[62]);
	logging_uint32_t_to_big_endian(vs_global.water_out_dreapta, &packet[66]);
	logging_uint32_t_to_big_endian(vs_global.aer_in_stanga, &packet[70]);
	logging_uint32_t_to_big_endian(vs_global.aer_in_dreapta, &packet[74]);
	logging_uint32_t_to_big_endian(vs_global.aer_out_stanga, &packet[78]);
	logging_uint32_t_to_big_endian(vs_global.aer_out_dreapta, &packet[82]);
	logging_uint32_t_to_big_endian(vs_global.manometru_stanga, &packet[86]);
	logging_uint32_t_to_big_endian(vs_global.manometru_dreapta, &packet[90]);
	logging_uint32_t_to_big_endian(vs_global.anemometru_stanga, &packet[94]);
	logging_uint32_t_to_big_endian(vs_global.anemometru_dreapta, &packet[98]);

	/* TODO: DAT */

	vs_give_mutex();

	return 0;
}

/**
 * This function converts a byte array to a hex string with a null terminator.
 */
static size_t
vs_packet_to_hexstr(uint8_t *packet, size_t packet_size, char *hexstr,
		     size_t hexstr_size)
{
	size_t i, k, hexstr_len;

	k = 0;
	for (i = 0, k = 0; i < packet_size && k < hexstr_size; i++) {
		hexstr[k++] = vs_get_hex_digit(packet[i] / 0x10);
		if (k < hexstr_size)
			hexstr[k++] = vs_get_hex_digit(packet[i] % 0x10);
	}

	hexstr_len = vs_min(k, hexstr_size - 1);
	hexstr[hexstr_len] = '\0';

	return hexstr_len;
}

static char
vs_get_hex_digit(uint8_t digit)
{
	if (digit < 10)
		return digit + '0';
	return digit - 10 + 'a';
}

/**
 * @return 0 on success and 1 otherwise
 */
static int
vs_take_mutex(void)
{
	if (xSemaphoreTake(vs_mutex, VS_MUTEX_TIMEOUT) != pdTRUE) {
		ESP_LOGW(VS_LOG_TAG,
			 "Failed to take the vehicle status mutex\n");
		return 1;
	}

	return 0;
}

static void
vs_give_mutex(void)
{
	xSemaphoreGive(vs_mutex);
}

static void
vs_update_ecu0(twai_message_t *msg)
{
	vs_global.rpm = msg->data[0];
	vs_global.curr_gear = msg->data[1];
	vs_global.tps = msg->data[2];
	vs_global.oil_pres = msg->data[3];
	vs_global.coolant_temp = msg->data[4];
	vs_global.fuel_pres = msg->data[5];
	vs_global.lambda = msg->data[6];
	vs_global.iat = msg->data[7];
}

static void
vs_update_ecu1(twai_message_t *msg)
{
	vs_global.egt1 = msg->data[0];
	vs_global.egt2 = msg->data[1];
	vs_global.egt3 = msg->data[2];
	vs_global.egt4 = msg->data[3];
	vs_global.vehicle_speed = msg->data[4];
	vs_global.map = msg->data[5];
	vs_global.bps = msg->data[6];
	vs_global.oil_temp = msg->data[7];
}

static void
vs_update_vdu0(twai_message_t *msg)
{
	vs_global.susLF = msg->data[0];
	vs_global.susRF = msg->data[1];
	vs_global.susLR = msg->data[2];
	vs_global.susRR = msg->data[3];

	/* big endian format */
	vs_global.accel = (((uint16_t) msg->data[4]) << 8) | msg->data[5];
	vs_global.gyro = (((uint16_t) msg->data[6]) << 8) | msg->data[7];
}

static void
vs_update_vdu1(twai_message_t *msg)
{
	vs_global.tyre_presLF = msg->data[4];
	vs_global.tyre_presRF = msg->data[5];
	vs_global.tyre_presLR = msg->data[6];
	vs_global.tyre_presRR = msg->data[7];
}

static void
vs_update_pmu0(twai_message_t *msg)
{
	vs_global.lv_bat_volts = msg->data[0];
	vs_global.in_voltage = msg->data[1];
	vs_global.in_cur_bat = msg->data[2];
	vs_global.in_cur_alt = msg->data[3];
}

static void
vs_update_pmu1(twai_message_t *msg)
{
	vs_global.output_current_0 = msg->data[0];
	vs_global.output_current_1 = msg->data[1];
	vs_global.output_current_2 = msg->data[2];
	vs_global.output_current_3 = msg->data[3];
	vs_global.output_current_4 = msg->data[4];
	vs_global.output_current_5 = msg->data[5];
	vs_global.output_current_6 = msg->data[6];
	vs_global.output_current_7 = msg->data[7];
}

static void
vs_update_pmu2(twai_message_t *msg)
{
	vs_global.output_voltage_0 = msg->data[0];
	vs_global.output_voltage_1 = msg->data[1];
	vs_global.output_voltage_2 = msg->data[2];
	vs_global.output_voltage_3 = msg->data[3];
	vs_global.output_voltage_4 = msg->data[4];
	vs_global.output_voltage_5 = msg->data[5];
	vs_global.output_voltage_6 = msg->data[6];
	vs_global.output_voltage_7 = msg->data[7];
}

static void
vs_update_dash(twai_message_t *msg)
{
	vs_global.dash_selector = msg->data[0];
}

static void
vs_update_scs(twai_message_t *msg)
{
	vs_global.scs = msg->data[0];
}

static void
vs_update_dat0(twai_message_t *msg)
{
	/* TODO */
}

static void
vs_update_dat1(twai_message_t *msg)
{
	/* TODO */
}

static void
vs_update_dat2(twai_message_t *msg)
{
	/* TODO */
}

static void
vs_update_dat3(twai_message_t *msg)
{
	/* TODO */
}

static void
vs_update_racire0(twai_message_t *msg)
{
	vs_global.water_in_stanga = ((uint32_t)msg->data[3] << 24) |
				    ((uint32_t)msg->data[2] << 16) |
				    ((uint32_t)msg->data[1] << 8) |
				    ((uint32_t)msg->data[0]);
	vs_global.water_in_dreapta = ((uint32_t)msg->data[7] << 24) |
				     ((uint32_t)msg->data[6] << 16) |
				     ((uint32_t)msg->data[5] << 8) |
				     ((uint32_t)msg->data[4]);
}

static void
vs_update_racire1(twai_message_t *msg)
{
	vs_global.water_out_stanga = ((uint32_t)msg->data[3] << 24) |
				     ((uint32_t)msg->data[2] << 16) |
				     ((uint32_t)msg->data[1] << 8) |
				     ((uint32_t)msg->data[0]);
	vs_global.water_out_dreapta = ((uint32_t)msg->data[7] << 24) |
				      ((uint32_t)msg->data[6] << 16) |
				      ((uint32_t)msg->data[5] << 8) |
				      ((uint32_t)msg->data[4]);
}

static void
vs_update_racire2(twai_message_t *msg)
{
	vs_global.aer_in_stanga = ((uint32_t)msg->data[3] << 24) |
				  ((uint32_t)msg->data[2] << 16) |
				  ((uint32_t)msg->data[1] << 8) |
				  ((uint32_t)msg->data[0]);
	vs_global.aer_in_dreapta =((uint32_t)msg->data[7] << 24) |
				  ((uint32_t)msg->data[6] << 16) |
				  ((uint32_t)msg->data[5] << 8) |
				  ((uint32_t)msg->data[4]);
}

static void
vs_update_racire3(twai_message_t *msg)
{
	vs_global.aer_out_stanga = ((uint32_t)msg->data[3] << 24) |
				   ((uint32_t)msg->data[2] << 16) |
				   ((uint32_t)msg->data[1] << 8) |
				   ((uint32_t)msg->data[0]);
	vs_global.aer_out_dreapta = ((uint32_t)msg->data[7] << 24) |
				    ((uint32_t)msg->data[6] << 16) |
				    ((uint32_t)msg->data[5] << 8) |
				    ((uint32_t)msg->data[4]);
}

static void
vs_update_racire4(twai_message_t *msg)
{
	vs_global.manometru_stanga = ((uint32_t)msg->data[3] << 24) |
				     ((uint32_t)msg->data[2] << 16) |
				     ((uint32_t)msg->data[1] << 8) |
				     ((uint32_t)msg->data[0]);
	vs_global.manometru_dreapta = ((uint32_t)msg->data[7] << 24) |
				      ((uint32_t)msg->data[6] << 16) |
				      ((uint32_t)msg->data[5] << 8) |
				      ((uint32_t)msg->data[4]);
}

static void
vs_update_racire5(twai_message_t *msg)
{
	vs_global.anemometru_stanga = ((uint32_t)msg->data[3] << 24) |
				      ((uint32_t)msg->data[2] << 16) |
				      ((uint32_t)msg->data[1] << 8) |
				      ((uint32_t)msg->data[0]);
	vs_global.anemometru_dreapta = ((uint32_t)msg->data[7] << 24) |
				       ((uint32_t)msg->data[6] << 16) |
				       ((uint32_t)msg->data[5] << 8) |
				       ((uint32_t)msg->data[4]);
}

static inline size_t
vs_min(size_t x, size_t y)
{
	return x < y ? x : y;
}

