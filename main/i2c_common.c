#include "i2c_common.h"
#include "esp_log.h"
#include "esp_check.h"

static const char *TAG = "i2c_common";
static i2c_master_bus_handle_t s_bus_handle = NULL;

esp_err_t i2c_common_init(gpio_num_t sda_gpio, gpio_num_t scl_gpio) {
    if (s_bus_handle != NULL) {
        ESP_LOGW(TAG, "I2C bus already initialized");
        return ESP_OK;
    }

    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = scl_gpio,
        .sda_io_num = sda_gpio,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,   // Включаем внутренние подтяжки
    };

    esp_err_t ret = i2c_new_master_bus(&bus_config, &s_bus_handle);
    ESP_RETURN_ON_ERROR(ret, TAG, "Failed to create I2C master bus");

    ESP_LOGI(TAG, "I2C master bus initialized (SDA=%d, SCL=%d)", sda_gpio, scl_gpio);
    return ESP_OK;
}

i2c_master_bus_handle_t i2c_common_get_bus(void) {
    return s_bus_handle;
}

void i2c_common_scan_bus(void) {
    if (s_bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C bus not initialized, cannot scan");
        return;
    }

    ESP_LOGI(TAG, "===== I2C Bus Scan Start =====");
    uint8_t found = 0;

    for (uint8_t addr = 1; addr < 127; addr++) {
        // i2c_master_probe безопасно пингует устройство, не вешая шину
        esp_err_t ret = i2c_master_probe(s_bus_handle, addr, 50);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "✓ Found device at 0x%02X", addr);
            found++;
        }
    }

    if (found == 0) {
        ESP_LOGW(TAG, "⚠ No devices found on I2C bus!");
    } else {
        ESP_LOGI(TAG, "Scan complete. Found %d device(s)", found);
    }
    ESP_LOGI(TAG, "===== I2C Bus Scan End =====");
}