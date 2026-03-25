#include "utility.h"
#include "GPIO.h"
#include "i2c.h"
#include "wifi.h"
esp_err_t configure_internal_state() {
  struct i2c_handles OLED_handlers = init_OLED();
  display = heap_caps_maloc(sizeof(lv_disp_t), MALLOC_CAP_INTERNAL);
  strncpy(display, ssd1306_driver_init(OLED_handlers), sizeof(lv_disp_t));
  current = heap_caps_maloc(sizeof(current_state), MALLOC_CAP_INTERNAL);
  create_screens();
  current->c_state = ARP_OPT;
  current->c_menu = MAIN;
  start_up_menu();
  update_opt();
}

esp_err_t updating_internal(enum button_event event) {
  if (event == UP_EVENT)
    current->c_state = (current + 1) % MAIN_OPT;
  else if (event == DOWN_EVENT)
    current->c_state = (current - 1) % MAIN_OPT;
  else if (event == SELECT_EVENT)
    change_screen(current->c_menu);
  return ESP_OK;
}

esp_err_t disconfigure_internal_state() {
  heap_caps_free(display);
  heap_caps_free(current);
  heap_caps_free(create_screens());
}
esp_err_t disable_wifi() { return ESP_OK; }
