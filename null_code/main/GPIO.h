#ifndef GPIO_H 
#define GPIO_H 
#include "utility.h"
#include "driver/gpio.h"
enum button_event{
	UP_EVENT = CONFIG_UP_BUTTON_GPIO, 
	SELECT_EVENT = CONFIG_SELECT_BUTTON_GPIO, 
	DOWN_EVENT = CONFIG_DOWN_BUTTON_GPIO
};

#define BIT_MASK(x) (uint64_t)((1<< x));
static int GPIOS[] = {CONFIG_UP_BUTTON_GPIO, CONFIG_DOWN_BUTTON_GPIO, CONFIG_SELECT_BUTTON_GPIO}; 
esp_err_t init_buttons();  

#endif 
