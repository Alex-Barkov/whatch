#pragma once

#include "driver/i2c_master.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Инициализация I2C master шины
 * @param sda_gpio Номер GPIO для SDA
 * @param scl_gpio Номер GPIO для SCL
 * @return ESP_OK при успехе
 */
esp_err_t i2c_common_init(gpio_num_t sda_gpio, gpio_num_t scl_gpio);

/**
 * @brief Получить handle инициализированной шины
 * @return handle шины или NULL, если не инициализирована
 */
i2c_master_bus_handle_t i2c_common_get_bus(void);

/**
 * @brief Сканирование шины I2C и вывод найденных устройств в лог
 */
void i2c_common_scan_bus(void);

#ifdef __cplusplus
}
#endif