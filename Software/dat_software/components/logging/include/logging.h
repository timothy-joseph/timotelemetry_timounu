#ifndef LOGGING_H
#define LOGGING_H

#include <driver/twai.h>

/* initialise rtc and microsd before initialising the logging component */
void logging_init_component(void);
void logging_log_can_msg_to_sd(const twai_message_t *msg);
/* microseconds since logging started */
uint32_t logging_get_timestamp_us(void);
void logging_uint32_t_to_big_endian(uint32_t data, uint8_t *buff);
void logging_uint16_t_to_big_endian(uint16_t data, uint8_t *buff);

#endif

