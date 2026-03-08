#ifndef BUTTON_H
#define BUTTON_H
#include "utility.h"
#include "driver/gpio.h"
#define BIT_MASK(x) (uint64_t)((1<< x));
static int GPIOS[] = {CONFIG_UP_BUTTON_GPIO, CONFIG_DOWN_BUTTON_GPIO, CONFIG_SELECT_BUTTON_GPIO}; 
esp_err_t init_buttons();  

#endif 
