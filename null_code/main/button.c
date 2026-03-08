#include "button.h"
esp_err_t init_buttons()
{
struct gpio_config_t gpio_cfg = {
	.pin_bit_mask = 0, 
	.model = GPIO_MODE_INPUT,
	.pull_up_en = GPIO_PULLUP_ENABLE, // we need to pull up high since we don't have an external pull up resistor to +3v
	.pull_down_en = GPIO_PULLDOWN_DISABLE,
	.intr_type = GPIO_INTR_NEGEDGE 
	};	
	for(size_t i =0; i<3; i++){
		gpio_cfg.pin_bit_mask = BIT_MASK(GPIOS[i]);
		ESP_ERROR_CHECK(gpio_config(&gpio_cfg)); // Honestly this Line should work since in the source code for gpio_config it doesn't need an acutal copy of the struct 
	}	

} 
