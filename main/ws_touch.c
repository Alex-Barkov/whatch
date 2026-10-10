#include "ws_touch.h"
#include "board_config.h"
#include "i2c_manager.h"
#include "esp_log.h"
#include "esp_lcd_touch_cst9217.h"
#include "esp_lv_adapter.h"
#include "esp_check.h"            // <-- ДОБАВЛЕНО

static const char *TAG = "ws_touch";
static lv_indev_t *s_disp_indev = NULL;

esp_err_t ws_touch_init(lv_display_t *disp, lv_indev_t **out_indev) {
    i2c_master_bus_handle_t i2c_bus = i2c_manager_get_bus();
    ESP_RETURN_ON_FALSE(i2c_bus, ESP_ERR_INVALID_STATE, TAG, "I2C bus not initialized");

    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST9217_CONFIG();
    tp_io_config.scl_speed_hz = WS_I2C_CLK_SPEED;
    
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c(i2c_bus, &tp_io_config, &tp_io_handle), TAG, "Failed to create I2C IO for touch");

    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = WS_LCD_H_RES,
        .y_max = WS_LCD_V_RES,
        .rst_gpio_num = WS_TOUCH_RST,
        .int_gpio_num = WS_TOUCH_INT,
        .levels = { .reset = 0, .interrupt = 0 },
        .flags = { .swap_xy = 0, .mirror_x = 1, .mirror_y = 1 },
    };

    esp_lcd_touch_handle_t tp = NULL;
    ESP_RETURN_ON_ERROR(esp_lcd_touch_new_i2c_cst9217(tp_io_handle, &tp_cfg, &tp), TAG, "Failed to init CST9217");

    const esp_lv_adapter_touch_config_t touch_cfg = ESP_LV_ADAPTER_TOUCH_DEFAULT_CONFIG(disp, tp);
    s_disp_indev = esp_lv_adapter_register_touch(&touch_cfg);
    *out_indev = s_disp_indev;

    ESP_LOGI(TAG, "Touch initialized successfully");
    return ESP_OK;
}