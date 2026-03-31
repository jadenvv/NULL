#include "GPIO.h "
#include "hal/gpio_types.h"
static IR_descrp ir_des = {0};
static bool finished = false;
void recv(void *arg) {
  static int prev_state = 1;
  static int64_t burst = esp_timer_get_time();
  static int64_t post_burst = 0;

  int state = gpio_get_level(CONFIG_IR_RECV);
  if (state == 1 && prev_state == 0) {
    post_burst = esp_timer_get_time();
    ir_des.burst = post_burt - burst;
  }
  if (state == 0 && prev_state == 1) {
    ir_des.space = esp_timer_get_time() - post_burst;
    finished = true;
  }
  prev_state = state;
}
esp_err_t init_IR() {
  gpio_config_t gpio_cfg_recv = {.pin_bit_mask = CONFIG_IR_RECV,
                                 .mode = GPIO_MODE_INPUT,
                                 .pull_up_en = GPIO_PULLUP_ENABLE,
                                 .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                 .intr_type = GPIO_INTR_ANYEDGE};
  ESP_ERROR_CHECK(gpio_config(&gpio_cfg_recv));
  gpio_config_t gpio_cfg_send = {.pin_bit_mask = CONFIG_IR_SEND,
                                 .mode = GPIO_MODE_OUTPUT,
                                 .pull_up_en = GPIO_PULLUP_DISABLE,
                                 .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                 .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&gpio_cfg_send));
  gpio_set_level(CONFIG_IR_SEND, 0);
  gpio_isr_handler_add(CONFIG_IR_RECV, recv, (void *)1);
  return ESP_OK;
}

IR_descrp IR_recv() {

  while (finished != true)
    ;
  return ir_des;
}
static inline void timer_callback(void *args) {
  gpio_set_level(CONFIG_IR_SEND, 1);
  return;
}
void IR_send(IR_descrp desp) {
  esp_timer_handle_t timer_handle;
  esp_timer_handle_t space_handle;
  esp_timer_create_args_t timer_config = {.callback = timer_callback,
                                          .arg = (void *)1,
                                          .name = "IR_timer ",
                                          .skip_unhandled_events = false};
  esp_timer_create(&timer_config, &timer_handle);
  esp_timer_create_args_t space_config = {.callback = timer_callback,
                                          .arg = (void *)1,
                                          .name = "IR_timer ",
                                          .skip_unhandled_events = false};
  esp_timer_create(&space_config, &space_handle);
  // honestly might be an issue with timing
  while (true) {
    esp_timer_start_once(space_handle, ir_des.space);
    esp_timer_start_once(timer_handle, ir_des.burst);
  }
}
