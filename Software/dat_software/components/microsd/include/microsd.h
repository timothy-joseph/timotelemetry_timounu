#ifndef MICROSD_H
#define MICROSD_H

/**
 * This driver uses static global variables to keep track of the mounted card
 * and opened file
 */
/**
 * Function used to mount the sd card using the fat filesystem. After calling
 * this function, you can open files from the sd card by putting the prefix
 * BASE_SD_PATH to the file name like:
 * fopen(BASE_SD_PATH"/dat_logs", "ab");
 *
 * @return Same as esp_vfs_fat_sdmmc_mount
 */
int microsd_init_driver(void);

/**
 * Closes the open file and unmount the sd card
 *
 * @return Same as esp_vfs_fat_sdcard_unmount
 */
int microsd_stop_driver(void);

/**
 * Opens a file with the name as the date fetched from the rtc (TODO)
 *
 * @returns 0 on success and 1 otherwise
 */
int microsd_init_filename(void);

/**
 * Writes data to the previously opened file
 *
 * @returns 0 on success and 1 otherwise
 */
int microsd_write_to_log_file(const uint8_t *data, const size_t n);

#endif

