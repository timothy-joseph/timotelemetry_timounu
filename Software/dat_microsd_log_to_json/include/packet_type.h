#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H

struct log_packet_t {
	uint32_t timestamp;
	uint16_t id;
	uint8_t data_size;
	uint8_t data[8];
};

#endif

