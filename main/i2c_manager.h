#pragma once
#include "driver/i2c_master.h"
#include "esp_err.h"

esp_err_t i2c_manager_init(void);
i2c_master_bus_handle_t i2c_manager_get_bus(void);