#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: menu icon style
lv_style_t *get_style_menu_icon_style_MAIN_FOCUSED();
lv_style_t *get_style_menu_icon_style_MAIN_DEFAULT();
void add_style_menu_icon_style(lv_obj_t *obj);
void remove_style_menu_icon_style(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/