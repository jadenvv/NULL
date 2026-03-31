#ifndef LORA_RFID_H
#define LORA_RFID_H
#include "GPIO.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"
#include "hal/spi_types.h"
#include "sdkconfig.h"

spi_handles init_SPI_bus();
void init_RFID_LoRa();
uint8_t *recv_RFID();
esp_err_t trans_RFID(uint8_t *send);
#endif
