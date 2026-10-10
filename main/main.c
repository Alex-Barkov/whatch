#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
//#include "nvs_flash.h"
#include "lvgl.h"

#include "i2c_manager.h"
#include "rtc_pcf85063.h"
#include "ws_display.h"
#include "ws_touch.h"
#include "ws_sdcard.h"
#include "esp_lv_adapter.h"
#include "ui/ui.h"

static const char *TAG = "main";

void app_main(void) {
    //ESP_ERROR_CHECK(nvs_flash_init());
    lv_init(); // 1. Инициализация ядра LVGL

    // 2. Инициализируем I2C шину ОДИН раз для всех устройств
    ESP_ERROR_CHECK(i2c_manager_init());

    // 3. Инициализируем RTC ПОКА шина свободна от прерываний тачскрина
    //ESP_ERROR_CHECK(pcf85063_init());
    
    //struct tm t = pcf85063_get_date_time();
    //ESP_LOGI(TAG, "RTC Time: %04d-%02d-%02d %02d:%02d:%02d",
    //         t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

    // 4. Задержка 50 мс для стабилизации шины I2C перед подключением "шумных" устройств
    vTaskDelay(pdMS_TO_TICKS(50));

    // 5. Инициализируем Дисплей
    lv_display_t *disp = NULL;
    ESP_ERROR_CHECK(ws_display_init(&disp));
    ws_display_set_brightness(80);
    // 6. Инициализируем Тачскрин (он безопасно подключится к той же шине I2C)
    lv_indev_t *indev = NULL;
    ESP_ERROR_CHECK(ws_touch_init(disp, &indev));
    ESP_ERROR_CHECK(esp_lv_adapter_start());
    ESP_LOGI(TAG, "All peripherals initialized successfully!");

    // 7. Инициализируем SD карту
   // ws_sdcard_mount();

    ESP_LOGI(TAG, "All peripherals initialized successfully!");


    ui_init();

    // Основной цикл приложения
    while (1) {

         lv_timer_handler();
        
        vTaskDelay(pdMS_TO_TICKS(50));
        //vTaskDelay(pdMS_TO_TICKS(1000));
       // struct tm now = pcf85063_get_date_time();
       // ESP_LOGI(TAG, "Current RTC Time: %02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
    }
}