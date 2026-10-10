#include "ws_sdcard.h"
#include "board_config.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"

static const char *TAG = "ws_sd";
static sdmmc_card_t *s_sdcard = NULL;

esp_err_t ws_sdcard_mount(void) {
    if (s_sdcard != NULL) return ESP_OK;

    const esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    const sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    const sdmmc_slot_config_t slot_config = {
        .clk = WS_SD_CLK, .cmd = WS_SD_CMD, .d0 = WS_SD_D0,
        .d1 = GPIO_NUM_NC, .d2 = GPIO_NUM_NC, .d3 = GPIO_NUM_NC,
        .cd = SDMMC_SLOT_NO_CD, .wp = SDMMC_SLOT_NO_WP,
        .width = 1, .flags = 0,
    };

    esp_err_t ret = esp_vfs_fat_sdmmc_mount(WS_SD_MOUNT_POINT, &host, &slot_config, &mount_config, &s_sdcard);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "SD card mounted at %s", WS_SD_MOUNT_POINT);
    } else {
        ESP_LOGW(TAG, "SD card mount failed: %s (Normal if no card is inserted)", esp_err_to_name(ret));
    }
    return ret;
}