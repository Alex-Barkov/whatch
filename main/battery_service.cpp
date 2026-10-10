/*

#include "lvgl.h"
#include "screens.h"        // Если нужно обращаться к экранам
#include "bsp/esp-bsp.h"    // Или XPowersLib.h
#include <cstdio>
#include "driver/i2c_master.h"
#include "port_axp2101.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG "battery_service"

#define PMU_INPUT_PIN (gpio_num_t) CONFIG_PMU_INTERRUPT_PIN
#define PMU_INPUT_PIN_SEL (1ULL << PMU_INPUT_PIN)

#define I2C_MASTER_NUM (i2c_port_num_t) CONFIG_I2C_MASTER_PORT_NUM
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY
#define I2C_MASTER_SDA_IO (gpio_num_t) CONFIG_PMU_I2C_SDA
#define I2C_MASTER_SCL_IO (gpio_num_t) CONFIG_PMU_I2C_SCL
#define I2C_MASTER_TIMEOUT_MS 1000



static i2c_master_bus_handle_t i2c_bus_handle = NULL;
static i2c_master_dev_handle_t pmu_dev_handle = NULL;
static QueueHandle_t gpio_evt_queue = NULL;

extern esp_err_t pmu_init();
extern void pmu_isr_handler();


static void IRAM_ATTR pmu_irq_handler(void *arg) {
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}


// PMU read function using new API
int pmu_register_read(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len) {
    esp_err_t ret = i2c_master_transmit_receive(pmu_dev_handle, &regAddr, 1, data, len, I2C_MASTER_TIMEOUT_MS);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "PMU READ FAILED!");
        return -1;
    }
    return 0;
}

// PMU write function using new API
int pmu_register_write_byte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len) {
    uint8_t *buffer = (uint8_t *)malloc(len + 1);
    if (!buffer) return -1;
    buffer[0] = regAddr;
    memcpy(&buffer[1], data, len);

    esp_err_t ret = i2c_master_transmit(pmu_dev_handle, buffer, len + 1, I2C_MASTER_TIMEOUT_MS);
    free(buffer);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "PMU WRITE FAILED!");
        return -1;
    }
    return 0;
}

// PMU event task
static void pmu_hander_task(void *args) {
    while (1) {
        pmu_isr_handler();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


static uint8_t read_battery_level(void) {    
   //  return PMU.getBatteryLevel();
}

// Callback таймера LVGL (безопасно для UI!)
static void battery_update_cb(lv_timer_t *timer) {
    uint8_t level = read_battery_level();
    if (level > 100) level = 100;

    char buf[8];
    snprintf(buf, sizeof(buf), "%d%%", level);

    // 🔑 Обновляем через objects.battery
    if (objects.battery) {  // Проверка на NULL — защита от гонки инициализации
        lv_label_set_text(objects.battery, buf);

        // Цвет в зависимости от заряда
        lv_color_t color;
        if (level <= 20)      color = lv_color_hex(0xFF4444);
        else if (level <= 50) color = lv_color_hex(0xFFAA00);
        else                  color = lv_color_hex(0x44FF44);
        
        lv_obj_set_style_text_color(objects.battery, color, 0);
    }
}

// Запуск сервиса (вызывать ПОСЛЕ create_screens())
void battery_service_init(void) {
    // Обновление раз в 60 секунд
    lv_timer_create(battery_update_cb, 60000, NULL);
    
    // Можно сразу обновить при старте
    battery_update_cb(NULL);
}

*/











































