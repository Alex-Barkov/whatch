#pragma once
#include "esp_err.h"
#include <time.h>
#include <stdbool.h>


#define PCF85063_ADDR 0x51

esp_err_t pcf85063_init(void);
esp_err_t pcf85063_set_date_time(const struct tm *time_ptr);
struct tm pcf85063_get_date_time(void);
bool pcf85063_is_present(void);