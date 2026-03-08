#ifndef I2C
#define I2C
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_lcd_panel_io.h" 
#define 
static const char * TAG = "I2C_OLED"
struct i2c_handles {
	struct i2c_master_bus_handle_t bus_handler;
	struct i2c_master_dev_handle_t dev_handler;
}; 
struct i2_master_dev_handle_t init_OLED();
struct esp_lcd_panel_handel_t ssd1306_driver_init(struct i2c_master_dev_handle_t * dev_handle); 
#endif 
