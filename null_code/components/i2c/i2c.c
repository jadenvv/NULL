#include "i2c.h"
static bool flush(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t * edata, void *user_ctx)
{
	lv_disp_t * disp = (lv_disp_t *)user_ctx; 
	lvgl_port_flush_ready(disp);
	return false;

}
struct i2c_handles init_OLED()
{
	ESP_LOGI(TAG, "intializing i2c...");
	i2c_master_bus_config_t bus_config = {
		.i2c_port =  -1,
		.sda_io_num = CONFIG_I2C_MASTER_SDA, 
		.scl_io_num = CONFIG_I2C_MASTER_SCL,
		.clk_source = I2C_CLK_SRC_RC_FAST,
		.glitch_ignore_cnt= 7 ,//might change 
		.intr_priority=0 ,
		.trans_queue_depth = 7, 
		.flags.allow_pd = true 
		};	
	i2c_master_bus_handle_t bus_handle; 
	esp_err_t err = i2c_new_master_bus(&bus_config, &bus_handle); 
	ESP_ERROR_CHECK(err);
	i2c_device_config_t device_config = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = CONFIG_I2C_DEVICE_ADDRESS, 
		.scl_speed_hz = CONFIG_I2C_MASTER_FREQUENCY, 
		.scl_wait_us = 0, 
		.flags.disable_ack_check = false
		};
	i2c_master_dev_handle_t dev_handle; 
	ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &device_config, &dev_handle)); 
	return (struct i2c_handles){bus_handle,dev_handle} ;
}
//note : i don't know if lv_disp_t goes out of scope and gets cleaned up 
lv_disp_t * ssd1306_driver_init(struct i2c_handles handles) 
{
	esp_lcd_panel_io_i2c_config_t config = {
		.dev_addr = CONFIG_I2C_DEVICE_ADDRESS, 
		.scl_speed_hz = CONFIG_I2C_MASTER_FREQUENCY,  
		.control_phase_bytes = 1, 
		.dc_bit_offset = 6,
		.lcd_cmd_bits = 8, 
		.lcd_param_bits = 8

	};
	esp_lcd_panel_io_handle_t IO_handle = NULL;  
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v2(handles.bus_handler, &config,&IO_handle)); 
	esp_lcd_panel_dev_config_t panel_config = {
		.bits_per_pixel = 1,
		.reset_gpio_num = -1
	};
	esp_lcd_panel_handle_t panel_handle = NULL;
	ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(IO_handle, &panel_config, &panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    	ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

	ESP_LOGI(TAG, "successfully initialized i2c...");
	const lvgl_port_cfg_t lvgl_config = ESP_LVGL_PORT_INIT_CONFIG();	
	lvgl_port_init(&lvgl_config);
	const lvgl_port_display_cfg_t disp_config = {
		.io_handle = IO_handle,
		.panel_handle = panel_handle,
		.buffer_size = CONFIG_HEIGHT_OLED * CONFIG_WIDTH_OLED,
		.hres = CONFIG_HEIGHT_OLED, 
		.vres = CONFIG_WIDTH_OLED,
		.monochrome = true, 
		.rotation = {
			.swap_xy = false, 
			.mirror_x = false, 
			.mirror_y = false 
		}
		};
	lv_disp_t * disp = lvgl_port_add_disp(&disp_config);
	const esp_lcd_panel_io_callbacks_t cbs = {
		.on_color_trans_done = flush,  
	};
	esp_lcd_panel_io_register_event_callbacks(IO_handle, &cbs, disp);
	ESP_LOGI(TAG, "initialized the LVGL display Handle");
	return disp;
}

