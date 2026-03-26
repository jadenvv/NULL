#ifndef DTYPES
#define DTYPES
#include "sdkconfig.h"
enum menu { MAIN, ARP, RFID, IR };
enum option_main { ARP_OPT, IR_OPT, RFID_OPT, LORA_OPT };
struct current_state {
  enum option_main c_state;
  enum menu c_menu;
};
// GPIO Dtypes
enum button_event {
  UP_EVENT = CONFIG_UP_BUTTON_GPIO,
  SELECT_EVENT = CONFIG_SELECT_BUTTON_GPIO,
  DOWN_EVENT = CONFIG_DOWN_BUTTON_GPIO
};

#endif
