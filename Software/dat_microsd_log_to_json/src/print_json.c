/**
 * TODO: figure out if i'm decoding these right
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#include <packet_type.h>
#include <print_json.h>

#define ECU0_ID 0x3e8
#define ECU1_ID 0x3e9
#define VDU0_ID 0x3ea
#define VDU1_ID 0x3ec
#define PMU0_ID 0x3ed
#define PMU1_ID 0x3ee
#define PMU2_ID 0x3ef
#define DASH_ID 0x384
#define SCS_ID  0x3f0
#define DAT0_ID 0x10
#define DAT1_ID 0x11
#define DAT2_ID 0x12
#define DAT3_ID 0x13

static void print_json_for_ecu0_id(struct log_packet_t *packet);
static void print_json_for_ecu1_id(struct log_packet_t *packet);
static void print_json_for_vdu0_id(struct log_packet_t *packet);
static void print_json_for_vdu1_id(struct log_packet_t *packet);
static void print_json_for_pmu0_id(struct log_packet_t *packet);
static void print_json_for_pmu1_id(struct log_packet_t *packet);
static void print_json_for_pmu2_id(struct log_packet_t *packet);
static void print_json_for_dash_id(struct log_packet_t *packet);
static void print_json_for_scs_id(struct log_packet_t *packet);
static void print_json_for_dat0_id(struct log_packet_t *packet);
static void print_json_for_dat1_id(struct log_packet_t *packet);
static void print_json_for_dat2_id(struct log_packet_t *packet);
static void print_json_for_dat3_id(struct log_packet_t *packet);

int
print_json_for_packet(struct log_packet_t *packet)
{
	printf("\t{\n\t\t\"timestamp\": %d", packet->timestamp);
	switch (packet->id) {
	case ECU0_ID:
		print_json_for_ecu0_id(packet);
		break;
	case ECU1_ID:
		print_json_for_ecu1_id(packet);
		break;
	case VDU0_ID:
		print_json_for_vdu0_id(packet);
		break;
	case VDU1_ID:
		print_json_for_vdu1_id(packet);
		break;
	case PMU0_ID:
		print_json_for_pmu0_id(packet);
		break;
	case PMU1_ID:
		print_json_for_pmu1_id(packet);
		break;
	case PMU2_ID:
		print_json_for_pmu2_id(packet);
		break;
	case DASH_ID:
		print_json_for_dash_id(packet);
		break;
	case SCS_ID:
		print_json_for_scs_id(packet);
		break;
	case DAT0_ID:
		print_json_for_dat0_id(packet);
		break;
	case DAT1_ID:
		print_json_for_dat1_id(packet);
		break;
	case DAT2_ID:
		print_json_for_dat2_id(packet);
		break;
	case DAT3_ID:
		print_json_for_dat3_id(packet);
		break;
	default:
		fprintf(stderr, "Unrecognized id: %" PRIx16 "\n", packet->id);
		return 1;
	}
	printf("\t}");

	return 0;
}

static void
print_json_for_ecu0_id(struct log_packet_t *packet)
{
	uint16_t rpm;
	uint8_t curr_gear, tps, oil_pres, coolant_temp;
	double fuel_pres, lambda, iat;

	rpm = packet->data[0] * 100;
	curr_gear = packet->data[1];
	tps = packet->data[2];
	oil_pres = packet->data[3];
	coolant_temp = packet->data[4];
	fuel_pres = ((double)packet->data[5]) * pow(2, -3);
	lambda = ((double)packet->data[6]) * pow(2, -7);
	iat = ((double)packet->data[7]) * pow(2, -3);

	printf(",\n\t\t\"rpm\": %" PRIu16 ",\n\t\t\"curr_gear\": %" PRIu8
	       ",\n\t\t\"tps\": %" PRIu8 ",\n\t\t\"oil_pres\": %" PRIu8
	       ",\n\t\t\"coolant_temp\": %" PRIu8
	       ",\n\t\t\"fuel_pres\": %f,\n\t\t\"lambda\": %f,\n\t\t\"iat\": %f\n",
	       rpm, curr_gear, tps, oil_pres, coolant_temp, fuel_pres, lambda,
	       iat);
}

static void
print_json_for_ecu1_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	double egt1, egt2, egt3, egt4;
	uint8_t vehicle_speed, map, bps, oil_temp;

	egt1 = (packet->data[0] * pow(2, -4)) * 100;
	egt2 = (packet->data[1] * pow(2, -4)) * 100;
	egt3 = (packet->data[2] * pow(2, -4)) * 100;
	egt4 = (packet->data[3] * pow(2, -4)) * 100;
	vehicle_speed = packet->data[4];
	map = packet->data[5];
	bps = packet->data[6];
	oil_temp = packet->data[7];

	printf
	    (",\n\t\t\"egt1\": %f,\n\t\t\"egt2\": %f,\n\t\t\"egt3\": %f,\n\t\t\"egt4\": %f,\n\t\t\"vehicle_speed\": %"
	     PRIu8 ",\n\t\t\"map\": %" PRIu8 ",\n\t\t\"bps\": %" PRIu8
	     ",\n\t\t\"oil_temp\": %" PRIu8 "\n", egt1, egt2, egt3, egt4,
	     vehicle_speed, map, bps, oil_temp);

}

static void
print_json_for_vdu0_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	uint8_t susLF, susRF, susLR, susRR;
	uint16_t accel, gyro;

	susLF = packet->data[0];
	susRF = packet->data[1];
	susLR = packet->data[2];
	susRR = packet->data[3];
	accel = (((uint16_t) packet->data[4]) << 8) | packet->data[5];
	gyro = (((uint16_t) packet->data[6]) << 8) | packet->data[7];

	printf(",\n\t\t\"susLF\": %" PRIu8 ",\n\t\t\"susRF\": %" PRIu8
	       ",\n\t\t\"susLR\": %" PRIu8 ",\n\t\t\"susRR\": %" PRIu8
	       ",\n\t\t\"accel\": %" PRIu16 ",\n\t\t\"gyro\": %" PRIu16 "\n",
	       susLF, susRF, susLR, susRR, accel, gyro);
}

static void
print_json_for_vdu1_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	double tyre_presLF, tyre_presRF, tyre_presLR, tyre_presRR;

	tyre_presLF = packet->data[4] * pow(2, -3);
	tyre_presRF = packet->data[5] * pow(2, -3);
	tyre_presLR = packet->data[6] * pow(2, -3);
	tyre_presRR = packet->data[7] * pow(2, -3);

	printf
	    (",\n\t\t\"tyre_presLF\": %f,\n\t\t\"tyre_presRF\": %f,\n\t\t\"tyre_presLR\": %f,\n\t\t\"tyre_presRR\": %f\n",
	     tyre_presLF, tyre_presRF, tyre_presLR, tyre_presRR);
}

static void
print_json_for_pmu0_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	double lv_bat_volts, in_voltage, in_cur_bat, in_cur_alt;

	lv_bat_volts = packet->data[0] * pow(2, -3);
	in_voltage = packet->data[1] * pow(2, -3);
	in_cur_bat = packet->data[2] * pow(2, -3);
	in_cur_alt = packet->data[3] * pow(2, -3);

	printf
	    (",\n\t\t\"lv_bat_volts\": %f,\n\t\t\"in_voltage\": %f,\n\t\t\"in_cur_bat\": %f,\n\t\t\"in_cur_alt\": %f\n",
	     lv_bat_volts, in_voltage, in_cur_bat, in_cur_alt);
}

static void
print_json_for_pmu1_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	uint8_t output_current_0, output_current_1, output_current_2;
	uint8_t output_current_3, output_current_4, output_current_5;
	uint8_t output_current_6, output_current_7;

	output_current_0 = packet->data[0];
	output_current_1 = packet->data[1];
	output_current_2 = packet->data[2];
	output_current_3 = packet->data[3];
	output_current_4 = packet->data[4];
	output_current_5 = packet->data[5];
	output_current_6 = packet->data[6];
	output_current_7 = packet->data[7];

	printf(",\n\t\t\"output_current_0\": %" PRIu8
	       ",\n\t\t\"output_current_1\": %" PRIu8
	       ",\n\t\t\"output_current_2\": %" PRIu8
	       ",\n\t\t\"output_current_3\": %" PRIu8
	       ",\n\t\t\"output_current_4\": %" PRIu8
	       ",\n\t\t\"output_current_5\": %" PRIu8
	       ",\n\t\t\"output_current_6\": %" PRIu8
	       ",\n\t\t\"output_current_7\": %" PRIu8 "\n", output_current_0,
	       output_current_1, output_current_2, output_current_3,
	       output_current_4, output_current_5, output_current_6,
	       output_current_7);
}

static void
print_json_for_pmu2_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	uint8_t output_voltage_0, output_voltage_1, output_voltage_2;
	uint8_t output_voltage_3, output_voltage_4, output_voltage_5;
	uint8_t output_voltage_6, output_voltage_7;

	output_voltage_0 = packet->data[0];
	output_voltage_1 = packet->data[1];
	output_voltage_2 = packet->data[2];
	output_voltage_3 = packet->data[3];
	output_voltage_4 = packet->data[4];
	output_voltage_5 = packet->data[5];
	output_voltage_6 = packet->data[6];
	output_voltage_7 = packet->data[7];

	printf(",\n\t\t\"output_voltage_0\": %" PRIu8
	       ",\n\t\t\"output_voltage_1\": %" PRIu8
	       ",\n\t\t\"output_voltage_2\": %" PRIu8
	       ",\n\t\t\"output_voltage_3\": %" PRIu8
	       ",\n\t\t\"output_voltage_4\": %" PRIu8
	       ",\n\t\t\"output_voltage_5\": %" PRIu8
	       ",\n\t\t\"output_voltage_6\": %" PRIu8
	       ",\n\t\t\"output_voltage_7\": %" PRIu8 "\n", output_voltage_0,
	       output_voltage_1, output_voltage_2, output_voltage_3,
	       output_voltage_4, output_voltage_5, output_voltage_6,
	       output_voltage_7);
}

static void
print_json_for_dash_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	uint8_t dash_selector;

	dash_selector = packet->data[0];

	printf(",\n\t\t\"dash_selector\": %" PRIu8 "\n", dash_selector);
}

static void
print_json_for_scs_id(struct log_packet_t *packet)
{
	/* TODO: print json */
	uint8_t scs;

	scs = packet->data[0];

	printf(",\n\t\t\"scs\": %" PRIu8 "\n", scs);
}

static void
print_json_for_dat0_id(struct log_packet_t *packet)
{
	(void)packet;
	/* (not used for the front-end) */
	/* TODO: print json */
	printf("\n");
}

static void
print_json_for_dat1_id(struct log_packet_t *packet)
{
	(void)packet;
	/* TODO */
	printf("\n");
}

static void
print_json_for_dat2_id(struct log_packet_t *packet)
{
	(void)packet;
	/* TODO */
	printf("\n");
}

static void
print_json_for_dat3_id(struct log_packet_t *packet)
{
	(void)packet;
	/* (used for the front-end once to sync the date) */
	/* TODO */
	printf("\n");
}
