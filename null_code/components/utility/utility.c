#include "utility.h"
#include "i2c.h"
esp_err_t configure_internal_state()
{
	struct i2c_handles OLED_handlers =init_OLED();
	display = heap_caps_maloc(sizeof(lv_disp_t),MALLOC_CAP_INTERNAL );		
	strncpy(display,ssd1306_driver_init(OLED_handlers),sizeof(lv_disp_t));
	current = heap_caps_maloc(sizeof(current_state),MALLOC_CAP_INTERNAL );		
	current->c_state = IR_EMU; 
	current->c_menu = MAIN; 	

} 
esp_err_t disconfigure_internal_state()
{
	heap_caps_free(display);
	heap_caps_free(current);

}
