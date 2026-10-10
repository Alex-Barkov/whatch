#include "gpio.h"
#include "driver/gpio.h"

/*static void button_isr_handler(void* arg) {

    //action_go_to_settings();
    ESP_LOGI(TAG, "button pressed");
    lv_scr_load_anim(objects.set_time, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
}
  */  


void init_calibration_button() {
    
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM_0),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);
    
    gpio_install_isr_service(0);
   // gpio_isr_handler_add(GPIO_NUM_0, button_isr_handler, NULL);
}
