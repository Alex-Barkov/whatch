file(GLOB_RECURSE UI_SOURCES "ui/*.c")

idf_component_register(
    SRCS 
        "main.c"
        "i2c_manager.c" 
        "rtc_pcf85063.c" 
        "ws_display.c" 
        "ws_touch.c" 
        "ws_sdcard.c" 
        ${UI_SOURCES}         # <--- ВОТ ЭТОЙ СТРОКИ НЕ ХВАТАЛО!
    INCLUDE_DIRS 
        "." 
        "ui"                  # <--- Добавьте "ui", чтобы main.c видел ui.h без сложных путей
    REQUIRES 
        driver 
        esp_lcd 
        esp_lcd_touch
        esp_lcd_touch_cst9217 
        esp_lcd_co5300 
        fatfs 
        lvgl 
        esp_lvgl_adapter
)