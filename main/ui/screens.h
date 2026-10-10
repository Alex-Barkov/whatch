#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETTINGS = 2,
    SCREEN_ID_MENU_TIME = 3,
    SCREEN_ID_SET_TIME = 4,
    SCREEN_ID_ALARM_CLOCK = 5,
    SCREEN_ID_ALARM_LIST = 6,
    SCREEN_ID_SET_DATE = 7,
    _SCREEN_ID_LAST = 7
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *settings;
    lv_obj_t *menu_time;
    lv_obj_t *set_time;
    lv_obj_t *alarm_clock;
    lv_obj_t *alarm_list;
    lv_obj_t *set_date;
    lv_obj_t *time;
    lv_obj_t *battery;
    lv_obj_t *date;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *time_1;
    lv_obj_t *hours;
    lv_obj_t *mins;
    lv_obj_t *button_set_time;
    lv_obj_t *obj2;
    lv_obj_t *hours_1;
    lv_obj_t *mins_1;
    lv_obj_t *button_set_time_1;
    lv_obj_t *obj3;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_settings();
void tick_screen_settings();

void create_screen_menu_time();
void tick_screen_menu_time();

void create_screen_set_time();
void tick_screen_set_time();

void create_screen_alarm_clock();
void tick_screen_alarm_clock();

void create_screen_alarm_list();
void tick_screen_alarm_list();

void create_screen_set_date();
void tick_screen_set_date();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

//
// Helper functions
//

lv_anim_t *get_anim();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/