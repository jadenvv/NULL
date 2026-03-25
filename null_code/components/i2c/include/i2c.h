#ifndef I2C
#define I2C
#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_ssd1306.h"
#include "esp_lvgl_port.h"
#include "utility.h"
#include <stdbool.h>
#define NUM_OF_SCR 4
#define MAIN_OPT 4
static enum menu *all_menus = [ MAIN, ARP, RFID, IR ];
static enum option_main *menu_opt = [ ARP_OPT, IR_OPT, RFID_OPT, LORA_OPT ];
static const char *TAG = "I2C_OLED";
struct i2c_handles {
  i2c_master_bus_handle_t bus_handler;
  i2c_master_dev_handle_t dev_handler;
};
struct {
  lv_obj_t *obj;
  bool active;
} lv_tracking;
struct {
  enum menu cur;
  lv_tracking track;
} state;
struct {
  enum option_main cur;
  lv_tracking track;
} state_opt;
struct i2c_handles init_OLED();
lv_disp_t *ssd1306_driver_init(struct i2c_handles dev_handle);
state_opt *menu_init_arr();
esp_err_t start_up_menu();
esp_err_t change_screen();
esp_err_t change_opt();

#endif
