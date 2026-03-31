#ifndef GPIO_H
#define GPIO_H
#include "driver/gpio.h"
#include "dtypes.h"
#include "esp_timer.h"
#include "utility.h"

esp_err_t init_buttons();
esp_err_t init_IRQ_RFID();
uint8_t addr_read(uint8_t address);
void addr_write(uint8_t address, uint8_t *buf, size_t sizeof_buf);
inline void soft_shudown_RFID();
void read_FIFO(uint8_t *buffer, size_t read_amount);
uint8_t *get_RFID();

esp_err_t init_IR();

#endif
