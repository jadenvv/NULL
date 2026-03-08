#include "null.h"
void app_main(void)
{
	//setup display	
	struct i2c_handles handles= init_OLED();
	lv_disp_t * disp_handler = ssd1306_driver_init(handles);

}
