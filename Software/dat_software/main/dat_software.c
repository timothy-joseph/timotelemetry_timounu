/**
 * The components of the DAT system are
 * 1. can component:
 *      - listen for incoming messages on can and send them via 3 channels:
 *      + using mqtt to a central server: 4 bytes representing ms since the can
 *      component was started, 4 bytes representing the can id, 1 byte
 *      representing data size, then a maximum of 8 data bytes (big endian
 *      format)
 *      + lora to the pits: format TODO
 *      + local logging: same format as the mqtt packets
 * 2. sim component:
 *      - commands can be enqueued (by the other components; mainly the can
 *      component) for the sim component to execute (using a 2 thread model:
 *      uart_rx_thread, uart_execute_commands_thread)
 *      - the uart_rx_thread has callbacks for connlost to reconnect to the
 *      mqtt server and for gpsinfo to extract the data and send it on the can
 *      network (using the can component; the can component will then receive
 *      this message and log it); another callback can be used to update the
 *      rtc to the current date on startup.
 * 3. rtc component:
 *      - this can be programmed using the correct date at the pits (if it is
 *      determined that the sim module isn't working correctly to extract the
 *      current date)
 *
 * Besides the can component, data is logged (using the same 3 methods) in
 * another place also: at the setup phase of the system. Here the current date
 * is logged using a pseudo can id in order to keep the packet consistent.
 *
 * The mqtt server translates the mqtt packets into json and then plots them on
 * a webpage. The json can be saved locally and loaded on the same webpage at a
 * later date.
 *
 * With the DAT system, there will also be a utility created for translating
 * the data logged unto the microsd card (mqtt packets) into json that can then
 * be loaded using the above mentioned website
 */
/* TODO (no order):
 * CAN component calls vs component which updates the rtc based on time
 * received from PMU
 * GPS for sim module
 */
#include <stdio.h>
#include <string.h>

#include <driver/gpio.h>
#include <driver/twai.h>
#include <driver/i2c.h>
#include <esp_vfs_fat.h>
#include <esp_err.h>
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <rtc_max31331.h>
#include <sim7600x_h.h>
#include <microsd.h>
#include <logging.h>
#include <can.h>
#include <vehicle_status.h>

#define DAT_TAG "DAT (main component)"

static void dat_setup(void);
/**
 * Function used to initialize all of the peripherals and threads of the dat
 * system.
 */
static void
dat_setup(void)
{
	struct rtc_calendar_t time;
	int ret;

	/* can pins setup */
	if (can_setup()) {
		ESP_LOGE(DAT_TAG, "Failed to install can driver\n");
		return;
	}

	/* RTC */
	if (rtc_i2c_init())
		ESP_LOGW(DAT_TAG, "failed to initialize the rtc\n");

	/* sim */
	if (sim_init_uart())
		ESP_LOGW(DAT_TAG, "Could not start the uart for the sim\n");
	if (sim_wait_for_rdy())
		ESP_LOGW(DAT_TAG,
			 "The sim module did not send the RDY flag in time\n");
	if (sim_setup_module())
		ESP_LOGW(DAT_TAG, "Could not connect to mqtt\n");

	if (microsd_init_driver())
		ESP_LOGW(DAT_TAG, "Could not initialize microsd\n");

	if (microsd_init_filename())
		ESP_LOGW(DAT_TAG, "Could not initialize microsd filename\n");

	logging_init_component();

	if (can_create_receive_thread() == NULL)
		ESP_LOGE(DAT_TAG, "Could not start the can thread\n");

	if (vs_init())
		ESP_LOGW(DAT_TAG,
			 "Could not initialize the vehicle_status component\n");
}

void
app_main(void)
{
	dat_setup();
}

