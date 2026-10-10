#include "ws_display.h"
#include "board_config.h"
#include "esp_log.h"
#include "esp_lcd_co5300.h"
// Убедись, что у тебя подключен компонент esp_lv_adapter от Waveshare
#include "esp_lv_adapter.h" 
#include "esp_check.h"            // <-- ДОБАВЛЕНО

static const char *TAG = "ws_display";
static esp_lcd_panel_handle_t s_panel_handle = NULL;
static esp_lcd_panel_io_handle_t s_io_handle = NULL;
static uint8_t s_brightness = 255;

static const co5300_lcd_init_cmd_t lcd_init_cmds[] = {
    {0xFE, (uint8_t[]){0x20}, 1, 0}, {0x19, (uint8_t[]){0x10}, 1, 0},
    {0x1C, (uint8_t[]){0xA0}, 1, 0}, {0xFE, (uint8_t[]){0x00}, 1, 0},
    {0xC4, (uint8_t[]){0x80}, 1, 0}, {0x3A, (uint8_t[]){0x55}, 1, 0},
    {0x35, (uint8_t[]){0x00}, 1, 0}, {0x53, (uint8_t[]){0x20}, 1, 0},
    {0x51, (uint8_t[]){0xFF}, 1, 0}, {0x63, (uint8_t[]){0xFF}, 1, 0},
    {0x2A, (uint8_t[]){0x00, 0x06, 0x01, 0xD7}, 4, 0},
    {0x2B, (uint8_t[]){0x00, 0x00, 0x01, 0xD1}, 4, 600},
    {0x11, NULL, 0, 600}, {0x29, NULL, 0, 0},
};

esp_err_t ws_display_init(lv_display_t **out_disp) {
    // 1. СНАЧАЛА инициализируем SPI и панель (как было)
    const spi_bus_config_t buscfg = CO5300_PANEL_BUS_QSPI_CONFIG(
        WS_LCD_PCLK, WS_LCD_DATA0, WS_LCD_DATA1, WS_LCD_DATA2, WS_LCD_DATA3,
        WS_LCD_H_RES * WS_LCD_V_RES * WS_LCD_BPP / 8
    );
    ESP_RETURN_ON_ERROR(spi_bus_initialize(WS_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    esp_lcd_panel_io_spi_config_t io_config = CO5300_PANEL_IO_QSPI_CONFIG(WS_LCD_CS, NULL, NULL);
    io_config.trans_queue_depth = 10;
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)WS_LCD_SPI_NUM, &io_config, &s_io_handle), TAG, "IO init failed");

    co5300_vendor_config_t vendor_config = {
        .init_cmds = lcd_init_cmds,
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
        .flags = { .use_qspi_interface = 1 },
    };

    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = WS_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = WS_LCD_BPP,
        .vendor_config = &vendor_config,
    };

    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_co5300(s_io_handle, &panel_config, &s_panel_handle), TAG, "Panel init failed");
    
    esp_lcd_panel_reset(s_panel_handle);
    esp_lcd_panel_init(s_panel_handle);
    esp_lcd_panel_disp_on_off(s_panel_handle, true);

    // 2. 🆕 ИНИЦИАЛИЗАЦИЯ АДАПТЕРА (ЭТОГО НЕ ХВАТАЛО!)
    esp_lv_adapter_config_t adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    // Можно настроить дополнительные параметры, если нужно:
    // adapter_cfg.task_stack_size = 8192;
    // adapter_cfg.task_priority = 5;
    ESP_RETURN_ON_ERROR(esp_lv_adapter_init(&adapter_cfg), TAG, "Adapter init failed");

    // 3. Теперь регистрируем дисплей
    esp_lv_adapter_display_config_t disp_cfg = {
        .panel = s_panel_handle,
        .panel_io = s_io_handle,
        .profile = {
            .interface = ESP_LV_ADAPTER_PANEL_IF_OTHER,
            .rotation = ESP_LV_ADAPTER_ROTATE_0,
            .hor_res = WS_LCD_H_RES,
            .ver_res = WS_LCD_V_RES,
            .buffer_height = 50,
            .use_psram = true,
            .require_double_buffer = true,
        },
    };
    *out_disp = esp_lv_adapter_register_display(&disp_cfg);
    
    // 4. НЕ вызываем esp_lv_adapter_start() здесь!
    //    Это должно быть в app_main ПОСЛЕ инициализации тача
    
    ESP_LOGI(TAG, "Display initialized");
    return ESP_OK;
}

esp_err_t ws_display_set_brightness(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    
    s_brightness = (uint8_t)(percent * 255 / 100);
    uint32_t lcd_cmd = 0x51;
    lcd_cmd &= 0xff;
    lcd_cmd <<= 8;
    lcd_cmd |= 0x02 << 24;
    
    return esp_lcd_panel_io_tx_param(s_io_handle, lcd_cmd, &s_brightness, 1);
}