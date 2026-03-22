#ifndef UTILITY_H
#define UTILITY_H
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "sdkconfig.h"
#include <string.h>
enum option { ARP_OPT, IR_OPT, RFID_OPT, LORA_OPT };
enum menu { MAIN, ARP, RFID, IR };
struct current_state {
  enum option c_state;
  enum menu c_menu;
};
static lv_disp_t *display = NULL;
static struct current_state *current = NULL;
esp_err_t configure_internal_state();
esp_err_t disconfigure_internal_state();
#endif
