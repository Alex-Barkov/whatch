#include "i2c_manager.h"
#include "board_config.h"
#include "esp_log.h"

static const char *TAG = "i2c_mgr";
static i2c_master_bus_handle_t s_i2c_bus = NULL;
static bool s_is_init = false;

esp_err_t i2c_manager_init(void) {
    if (s_is_init) {
        return ESP_OK; // Уже инициализировано, возвращаем успех
    }

    i2c_master_bus_config_t bus_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .sda_io_num = WS_I2C_SDA,
        .scl_io_num = WS_I2C_SCL,
        .i2c_port = WS_I2C_NUM,
        .flags.enable_internal_pullup = true, // Критически важно для стабильности
    };

    esp_err_t ret = i2c_new_master_bus(&bus_cfg, &s_i2c_bus);
    if (ret == ESP_OK) {
        s_is_init = true;
        ESP_LOGI(TAG, "I2C bus initialized (SDA:%d, SCL:%d)", WS_I2C_SDA, WS_I2C_SCL);
    } else {
        ESP_LOGE(TAG, "Failed to init I2C bus: %s", esp_err_to_name(ret));
    }
    return ret;
}

i2c_master_bus_handle_t i2c_manager_get_bus(void) {
    return s_i2c_bus;
}