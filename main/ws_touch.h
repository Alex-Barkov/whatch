#pragma once
#include "esp_err.h"
#include "lvgl.h"

esp_err_t ws_touch_init(lv_display_t *disp, lv_indev_t **out_indev);