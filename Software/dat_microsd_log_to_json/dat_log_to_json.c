/**
 * TODO: add sync packet
 */
#include <stdio.h>
#include <stdint.h>

#include <packet_type.h>
#include <print_json.h>

static size_t fread_big_endian_uint32_t(uint32_t *ret, FILE *f);
static size_t fread_big_endian_uint16_t(uint16_t *ret, FILE *f);
static size_t fread_big_endian_uint8_t(uint8_t *ret, FILE *f);
long get_seek_end(FILE *file);
static int parse_single_log(FILE *file);
static void parse(FILE *file);

static size_t
fread_big_endian_uint32_t(uint32_t *ret, FILE *f)
{
	size_t fread_ret = 0;
	uint8_t buff[4];

	fread_ret = fread(buff, sizeof(*buff), 4, f);
	if (fread_ret != 4)
		return 0;
	
	*ret = (((uint32_t)buff[0]) << 24) | (((uint32_t)buff[1]) << 16) |
	       (((uint32_t)buff[2]) << 8) | ((uint32_t)buff[3]);
	
	return 1;
}

static size_t
fread_big_endian_uint16_t(uint16_t *ret, FILE *f)
{
	size_t fread_ret = 0;
	uint8_t buff[2];

	fread_ret = fread(buff, sizeof(*buff), 2, f);
	if (fread_ret != 2)
		return 0;
	
	*ret = (((uint16_t)buff[0]) << 8) | ((uint16_t)buff[1]);
	
	return 1;
}

static size_t
fread_big_endian_uint8_t(uint8_t *ret, FILE *f)
{
	return fread(ret, sizeof(*ret), 1, f);
}

long
get_seek_end(FILE *file)
{
	long current_offset, seek_end_offset;

	current_offset = ftell(file);
	fseek(file, 0, SEEK_END);
	seek_end_offset = ftell(file);
	fseek(file, current_offset, SEEK_SET);

	return seek_end_offset;
}

static int
parse_single_log(FILE *file)
{
	struct log_packet_t packet;
	size_t fread_ret, i;

	fread_ret = fread_big_endian_uint32_t(&packet.timestamp, file);
	if (fread_ret == 0)
		return 1;

	fread_ret = fread_big_endian_uint16_t(&packet.id, file);
	if (fread_ret == 0)
		return 1;

	fread_ret = fread_big_endian_uint8_t(&packet.data_size, file);
	if (fread_ret == 0)
		return 1;

	for (i = 0; i < packet.data_size; i++) {
		fread_ret = fread_big_endian_uint8_t(&packet.data[i], file);
		if (fread_ret == 0)
			return 1;
	}

	(void)print_json_for_packet(&packet);

	return 0;
}

static void
parse(FILE *file)
{
	long seek_end_offset;
	int ret, ok_to_parse;

	seek_end_offset = get_seek_end(file);

	printf("[\n");

	ok_to_parse = 1;
	while (ok_to_parse) {
		/* TODO: find a way to know if i should print the comma or not
		 */
		if (parse_single_log(file))
			ok_to_parse = 0;

		if (ftell(file) != seek_end_offset)
			printf(",");

		printf("\n");
	}

	printf("]\n");
}

int
main(int argc, char *argv[])
{
	FILE *input_file;

	if (argc == 1)
		input_file = stdin;
	else
		input_file = fopen(argv[1], "rb");
	
	if (input_file == NULL) {
		fputs("Could open input file\n", stderr);
		return 1;
	}

	parse(input_file);

	return 0;
}

