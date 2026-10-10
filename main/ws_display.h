#pragma once
#include "esp_err.h"
#include "esp_lcd_panel_ops.h"
#include "lvgl.h"

esp_err_t ws_display_init(lv_display_t **out_disp);
esp_err_t ws_display_set_brightness(int percent);