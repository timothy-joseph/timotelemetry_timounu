#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <driver/gpio.h>
#include <driver/sdmmc_host.h>
#include <sdmmc_cmd.h>
#include <esp_vfs_fat.h>
#include <esp_err.h>
#include <esp_log.h>

#include <rtc_max31331.h>
#include <sim7600x_h.h>

#include "microsd.h"

#include "microsd_config.h"

static int microsd_build_filename_from_rtc(char *buf, size_t n);

static sdmmc_card_t *microsd_card;
static FILE *microsd_log_file;
char microsd_filename[30];

int
microsd_init_driver(void)
{
	sdmmc_host_t host_cfg = SDMMC_HOST_DEFAULT();
	sdmmc_slot_config_t slot_cfg = SDMMC_SLOT_CONFIG_DEFAULT();
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
		.format_if_mount_failed = MICROSD_FORMAT_IF_MOUNT_FAILED,
		.max_files = MICROSD_MAX_FILES,
		.allocation_unit_size = 16 * 1024
	};

	slot_cfg.width = 4;
	slot_cfg.clk = MICROSD_CLK;
	slot_cfg.cmd = MICROSD_CMD;
	slot_cfg.d0 = MICROSD_DAT0;
	slot_cfg.d1 = MICROSD_DAT1;
	slot_cfg.d2 = MICROSD_DAT2;
	slot_cfg.d3 = MICROSD_DAT3;

#if 0
	/* i have external pullups. it might be a good idea to turn internal ones off */
	slot_cfg.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
#endif

	return esp_vfs_fat_sdmmc_mount(MICROSD_BASE_PATH, &host_cfg, &slot_cfg,
				       &mount_config, &microsd_card);

}

int
microsd_stop_driver(void)
{
	fclose(microsd_log_file);
	return esp_vfs_fat_sdcard_unmount(MICROSD_BASE_PATH, microsd_card);
}

int
microsd_init_filename(void)
{
	int ret;

	ret = microsd_build_filename_from_rtc(microsd_filename,
					      sizeof(microsd_filename));
	if (!ret) {
		microsd_log_file = fopen(microsd_filename, MICROSD_FOPEN_MODE);
		if (microsd_log_file != NULL) {
			fclose(microsd_log_file);
			return 0;
		}
	}
	strncpy(microsd_filename, MICROSD_BASE_PATH"/"MICROSD_DEFAULT_FILENAME,
		sizeof(microsd_filename));
	return 0;
}

int
microsd_write_to_log_file(const uint8_t *data, const size_t n)
{
	FILE *f;
	size_t fwrite_ret;

#if 0
	if (microsd_log_file == NULL)
		return 1;
#endif
	if (data == NULL)
		return 1;
	

	/* without opening then closing the file, it doesn't get saved */
	f = fopen(microsd_filename, MICROSD_FOPEN_MODE);
	if (!f) {
		ESP_LOGW(MICROSD_TAG, "Could not open file\n");
		return 1;
	}
	fwrite_ret = fwrite(data, 1, n, f);
	fclose(f);
	if (fwrite_ret != n) {
		ESP_LOGW(MICROSD_TAG, "Could not write the required amount\n");
		return 1;
	}
	
	return 0;
}

/**
 * This function reads the calendar from the rtc then using snprintf writes at
 * most n bytes into buf representing the filename of the file to be opened.
 * The filename constructed represents the current date and time. The filename
 * also contains the whole path
 *
 * filename format: unix timestamp in hex
 * i couldn't make it more complicated because of the FAT32 8 character limit
 * for the filename
 *
 * @returns 0 if the filename was contructed correctly and 0 otherwise
 */
static int
microsd_build_filename_from_rtc(char *buf, size_t n)
{
	struct rtc_calendar_t rtc_now;
	int32_t secs;
	int ret;

	ret = rtc_get_calendar(&rtc_now);
	if (ret)
		return ret;
	
	secs = rtc_calendar_to_unix_time_sec(&rtc_now);
	snprintf(buf, n, MICROSD_BASE_PATH"/%08"PRIx32, secs);
	puts(buf);

	return 0;
}

