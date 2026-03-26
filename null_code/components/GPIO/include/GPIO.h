#ifndef GPIO_H
#define GPIO_H
#include "driver/gpio.h"
#include "dtypes.h"
#include "utility.h"

#define BIT_MASK(x) (uint64_t)((1 << x));

esp_err_t init_buttons();
esp_err_t init_IR();

#endif
