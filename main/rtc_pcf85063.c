#include "rtc_pcf85063.h"
#include "i2c_manager.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "board_config.h"

static const char *TAG = "rtc_pcf85063";
static i2c_master_dev_handle_t s_rtc_dev = NULL;

#define BIN_TO_BCD(bin)  ((((bin) / 10) << 4) | ((bin) % 10))
#define BCD_TO_BIN(bcd)  ((((bcd) >> 4) & 0x0F) * 10 + ((bcd) & 0x0F))

static esp_err_t pcf85063_read_regs(uint8_t start_reg, uint8_t *out_data, size_t len) {
    ESP_RETURN_ON_FALSE(s_rtc_dev, ESP_ERR_INVALID_STATE, TAG, "RTC not created");
    for (int attempt = 0; attempt < 3; attempt++) {
        esp_err_t ret = i2c_master_transmit(s_rtc_dev, &start_reg, 1, 100);
        if (ret != ESP_OK) { vTaskDelay(pdMS_TO_TICKS(5)); continue; }
        ret = i2c_master_receive(s_rtc_dev, out_data, len, 100);
        if (ret == ESP_OK) return ESP_OK;
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    return ESP_ERR_TIMEOUT;
}

static esp_err_t pcf85063_write_reg(uint8_t reg_addr, uint8_t value) {
    ESP_RETURN_ON_FALSE(s_rtc_dev, ESP_ERR_INVALID_STATE, TAG, "RTC not created");
    uint8_t buf[2] = {reg_addr, value};
    return i2c_master_transmit(s_rtc_dev, buf, 2, 100);
}

bool pcf85063_is_present(void) {
    i2c_master_bus_handle_t bus = i2c_manager_get_bus();
    if (!bus) return false;
    return (i2c_master_probe(bus, PCF85063_ADDR, 50) == ESP_OK);
}

esp_err_t pcf85063_init(void) {
    i2c_master_bus_handle_t bus = i2c_manager_get_bus();
    ESP_RETURN_ON_FALSE(bus, ESP_ERR_INVALID_STATE, TAG, "I2C bus not initialized");

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = PCF85063_ADDR,
        .scl_speed_hz = WS_I2C_CLK_SPEED,
        .flags = { .disable_ack_check = false },
    };

    ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus, &dev_cfg, &s_rtc_dev), TAG, "Failed to add RTC device");

    if (!pcf85063_is_present()) {
        ESP_LOGE(TAG, "RTC at 0x%02X is NOT responding!", PCF85063_ADDR);
        return ESP_ERR_NOT_FOUND;
    }

    // Сброс регистров: Control_1 (STOP=0), Control_2 (irq off), Offset, RAM
    ESP_RETURN_ON_ERROR(pcf85063_write_reg(0x00, 0x00), TAG, "Failed to write reg 0");
    ESP_RETURN_ON_ERROR(pcf85063_write_reg(0x01, 0x00), TAG, "Failed to write reg 1");
    ESP_RETURN_ON_ERROR(pcf85063_write_reg(0x02, 0x00), TAG, "Failed to write reg 2");
    ESP_RETURN_ON_ERROR(pcf85063_write_reg(0x03, 0x00), TAG, "Failed to write reg 3");

    ESP_LOGI(TAG, "RTC initialized successfully");
    return ESP_OK;
}

esp_err_t pcf85063_set_date_time(const struct tm *time_ptr) {
    ESP_RETURN_ON_FALSE(time_ptr && s_rtc_dev, ESP_ERR_INVALID_ARG, TAG, "Invalid args");
    
    pcf85063_write_reg(0x00, 0x20); // STOP = 1

    uint8_t buffer[8];
    buffer[0] = 0x04;
    buffer[1] = BIN_TO_BCD(time_ptr->tm_sec) & 0x7F;
    buffer[2] = BIN_TO_BCD(time_ptr->tm_min) & 0x7F;
    buffer[3] = BIN_TO_BCD(time_ptr->tm_hour) & 0x3F;
    buffer[4] = BIN_TO_BCD(time_ptr->tm_mday) & 0x3F;
    buffer[5] = BIN_TO_BCD(time_ptr->tm_wday) & 0x07;
    buffer[6] = BIN_TO_BCD(time_ptr->tm_mon + 1) & 0x1F; // 1-12
    buffer[7] = BIN_TO_BCD(time_ptr->tm_year % 100);

    ESP_RETURN_ON_ERROR(i2c_master_transmit(s_rtc_dev, buffer, 8, 100), TAG, "Failed to write time");
    return pcf85063_write_reg(0x00, 0x00); // STOP = 0 (запуск)
}

struct tm pcf85063_get_date_time(void) {
    struct tm t = {0};
    if (!s_rtc_dev) return t;

    uint8_t buffer[7] = {0};
    if (pcf85063_read_regs(0x04, buffer, sizeof(buffer)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read date/time");
        return t;
    }

    t.tm_sec  = BCD_TO_BIN(buffer[0] & 0x7F);
    t.tm_min  = BCD_TO_BIN(buffer[1] & 0x7F);
    t.tm_hour = BCD_TO_BIN(buffer[2] & 0x3F);
    t.tm_mday = BCD_TO_BIN(buffer[3] & 0x3F);
    t.tm_wday = buffer[4] & 0x07;
    t.tm_mon  = BCD_TO_BIN(buffer[5] & 0x1F) - 1; // 0-11 для struct tm
    t.tm_year = BCD_TO_BIN(buffer[6]) + 100;      // 2000+ год

    return t;
}