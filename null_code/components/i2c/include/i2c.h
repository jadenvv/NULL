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
static enum menu *all_menus = [ MAIN, ARP, RFID, IR ];
static const char *TAG = "I2C_OLED";
struct i2c_handles {
  i2c_master_bus_handle_t bus_handler;
  i2c_master_dev_handle_t dev_handler;
};
struct {
  enum menu cur;
  lv_obj_t *obj;
} state;
struct i2c_handles init_OLED();
lv_disp_t *ssd1306_driver_init(struct i2c_handles dev_handle);
esp_err_t start_up_menu(lv_disp_t *display) esp_err_t change_screen();
#endif
