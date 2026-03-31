#ifndef GPIO_H
#define GPIO_H
#include "driver/gpio.h"
#include "dtypes.h"
#include "utility.h"

esp_err_t init_buttons();
esp_err_t init_IR();
esp_err_t init_IRQ_RFID();
inline void soft_shudown_RFID();

#endif
