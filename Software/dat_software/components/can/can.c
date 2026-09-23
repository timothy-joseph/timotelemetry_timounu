#include <driver/gpio.h>
#include <driver/twai.h>
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <vehicle_status.h>
#include <logging.h>
#include <rtc.h>

#include "can.h"

#include "can_config.h"

static void can_receive_thread(void *arg);

/**
 * mutex used so 2 threads can't try to send something over can at the same time
 */
SemaphoreHandle_t can_mutex;

int
can_setup(void)
{
	int ret;
	twai_general_config_t general_cfg =
			TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN,
						    TWAI_MODE_NORMAL);
	twai_timing_config_t timing_cfg = {
		.clk_src = TWAI_CLK_SRC_APB,
		.brp = 10,
		.tseg_1 = 13,
		.tseg_2 = 2,
		.sjw = 1
	};
	twai_filter_config_t filter_cfg = TWAI_FILTER_CONFIG_ACCEPT_ALL();

	can_mutex = xSemaphoreCreateMutex();
	if (!can_mutex)
		return 1;
	
	ret = twai_driver_install(&general_cfg, &timing_cfg, &filter_cfg);
	if (ret) {
		ESP_LOGE(CAN_TAG, "%d\n", ret);
		return ret;
	}

	/*
	 * TODO: call function in here to startup the periodic timer for gps
	 * location updates instead of the sim driver's setup function
	 */

	return 0;
}

int
can_send_message(twai_message_t *msg)
{
	/* a mutex might actually not be needed */
	int ret;

	if (xSemaphoreTake(can_mutex, CAN_UNIVERSAL_TIMEOUT) != pdTRUE)
		return 1;

	ret = twai_transmit(msg, CAN_UNIVERSAL_TIMEOUT);

	xSemaphoreGive(can_mutex);

	return ret;
}

/**
 * This function is used to create the thread that receives all can messages.
 * it's stack and tcb are allocated statically.
 *
 * @return Something other than NULL on success and NULL on failure
 */
TaskHandle_t
can_create_receive_thread(void)
{
	static StaticTask_t tcb;
	static StackType_t stack[CAN_RX_THREAD_STACK_SIZE];

	return xTaskCreateStaticPinnedToCore(can_receive_thread, "can_rx",
					     CAN_RX_THREAD_STACK_SIZE, NULL,
					     CAN_RX_THREAD_PRIORITY, stack,
					     &tcb, tskNO_AFFINITY);
}

/**
 * This is the thread that receives all can messages. Every can message is
 * logged using the logging component
 */
static void
can_receive_thread(void *arg)
{
	(void)arg;

	twai_message_t msg;

	ESP_ERROR_CHECK(twai_start());

	while (1) {
		if (twai_receive(&msg, portMAX_DELAY) != ESP_OK)
			continue;

		vs_update_from_can_msg(&msg);
		logging_log_can_msg_to_sd(&msg);
	}
}

