#include "actions.h"
#include "screens.h"
#include "esp_log.h"
#include "rtc_pcf85063.h"
#include <time.h>
#include <sys/time.h>

#define TAG "NAV"

void action_go_to_settings(lv_event_t * e) {
    lv_screen_load_anim(objects.set_time , LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    ESP_LOGI("NAV", "Switched to Settings");
}

void action_go_to_main(lv_event_t * e) {
    lv_screen_load_anim(objects.main, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    ESP_LOGI("NAV", "Switched to Main");
}

void action_set_time_pressed(lv_event_t * e){
    struct tm timeinfo;
    timeinfo.tm_hour = lv_roller_get_selected(objects.hours);;   
    timeinfo.tm_min = lv_roller_get_selected(objects.mins);;
    timeinfo.tm_sec = 0;
    pcf85063_set_date_time(&timeinfo);
    time_t epoch_seconds = mktime(&timeinfo);

    struct timeval tv;
    tv.tv_sec = epoch_seconds;
    tv.tv_usec = 0;           
    settimeofday(&tv, NULL);

    ESP_LOGI("NAV", "Time set to %02d:%02d", timeinfo.tm_hour, timeinfo.tm_min); 
}
