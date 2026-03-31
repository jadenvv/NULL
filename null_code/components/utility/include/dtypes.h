#ifndef DTYPES
#define DTYPES
#include "sdkconfig.h"
enum menu { MAIN, ARP, RFID, IR };
enum option_main { ARP_OPT, IR_OPT, RFID_OPT, LORA_OPT };
struct current_state {
  enum option_main c_state;
  enum menu c_menu;
};
#define BIT_MASK(x) (uint64_t)((1 << x));
#define CMD_RFID(cont_bit, op)                                                 \
  cont_bit ? ((uint8_t)0xA0 | op) : ((uint8_t)0x80 | op)
#define ADDR_RFID(RW) ((uint8_t)0x20 | (RW << 7))
// a 1 is a read a 0 is a write
// im just defaulting to always having cont_bit for ADDR reading and writing
// GPIO Dtypes
enum button_event {
  UP_EVENT = CONFIG_UP_BUTTON_GPIO,
  SELECT_EVENT = CONFIG_SELECT_BUTTON_GPIO,
  DOWN_EVENT = CONFIG_DOWN_BUTTON_GPIO
};

#endif
