#include "GPIO.h"
void gpio_handler_function(void * gpio_number)
{
	uint32_t gpio = *(uint32_t *)gpio_number; 

}
esp_err_t init_buttons()
{
gpio_config_t gpio_cfg = {
	.pin_bit_mask = 0, 
	.mode = GPIO_MODE_INPUT,
	.pull_up_en = GPIO_PULLUP_ENABLE, // we need to pull up high since we don't have an external pull up resistor to +3v
	.pull_down_en = GPIO_PULLDOWN_DISABLE,
	.intr_type = GPIO_INTR_NEGEDGE 
	};	
	for(size_t i =0; i<3; i++){
		gpio_cfg.pin_bit_mask = BIT_MASK(GPIOS[i]);
		ESP_ERROR_CHECK(gpio_config(&gpio_cfg)); // Honestly this Line should work since in the source code for gpio_config it doesn't need an acutal copy of the struct 
	}
	gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
	for(size_t i=0; i<3; i++)
	{
		gpio_isr_handler_add(GPIOS[i], gpio_handler_function, (void*)&GPIOS[i]);

	}
	return ESP_OK;
} 
esp_err_t init_IR(){
	gpio_config_t gpio_cfg = {
		.pin_bit_mask = CONFIG_IR_RECV, 
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE, 
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_ANYEDGE 
	};	
	ESP_ERROR_CHECK(gpio_config(&gpio_cfg)); 	
	gpio_cfg = {
		.pin_bit_mask = CONFIG_IR_SEND, 
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE, 
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE
	};	
	ESP_ERROR_CHECK(gpio_config(&gpio_cfg)); 	

}
