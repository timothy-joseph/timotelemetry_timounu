/**
 * This component updates a global variable that represents the vehicle status
 * using the function vs_update_from_can_msg. This component provides a
 * periodic function to send the data using mqtt
 */
#ifndef VEHICLE_STATUS_H
#define VEHICLE_STATUS_H

#include <driver/twai.h>

struct vehicle_status_t {
	/* ECU0 */
	uint8_t rpm;
	uint8_t curr_gear;
	uint8_t tps;
	uint8_t oil_pres;
	uint8_t coolant_temp;
	uint8_t fuel_pres;
	uint8_t lambda;
	uint8_t iat;

	/* ECU1 */
	uint8_t egt1;
	uint8_t egt2;
	uint8_t egt3;
	uint8_t egt4;
	uint8_t vehicle_speed;
	uint8_t map;
	uint8_t bps;
	uint8_t oil_temp;

	/* VDU0 */
	uint8_t susLF;
	uint8_t susRF;
	uint8_t susLR;
	uint8_t susRR;
	uint16_t accel;
	uint16_t gyro;

	/* VDU1 */
	uint8_t tyre_presLF;
	uint8_t tyre_presRF;
	uint8_t tyre_presLR;
	uint8_t tyre_presRR;

	/* PMU0 */
	uint8_t lv_bat_volts;
	uint8_t in_voltage;
	uint8_t in_cur_bat;
	uint8_t in_cur_alt;

	/* PMU1 */
	uint8_t output_current_0;
	uint8_t output_current_1;
	uint8_t output_current_2;
	uint8_t output_current_3;
	uint8_t output_current_4;
	uint8_t output_current_5;
	uint8_t output_current_6;
	uint8_t output_current_7;

	/* PMU2 */
	uint8_t output_voltage_0;
	uint8_t output_voltage_1;
	uint8_t output_voltage_2;
	uint8_t output_voltage_3;
	uint8_t output_voltage_4;
	uint8_t output_voltage_5;
	uint8_t output_voltage_6;
	uint8_t output_voltage_7;

	/* DASH */
	uint8_t dash_selector;

	/* SCS */
	uint8_t scs;

	/* DAT0: TODO */
	/* DAT1: TODO */
	/* DAT2: TODO */
	/* DAT3: TODO */

	/* RACIRE */
	uint32_t water_in_stanga;
	uint32_t water_in_dreapta;
	uint32_t water_out_stanga;
	uint32_t water_out_dreapta;
	uint32_t aer_in_stanga;
	uint32_t aer_in_dreapta;
	uint32_t aer_out_stanga;
	uint32_t aer_out_dreapta;
	uint32_t manometru_stanga;
	uint32_t manometru_dreapta;
	uint32_t anemometru_stanga;
	uint32_t anemometru_dreapta;
};

int vs_init(void);
int vs_update_from_can_msg(twai_message_t *msg);

#endif

