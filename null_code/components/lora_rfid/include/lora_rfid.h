#ifndef LORA_RFID_H
#define LORA_RFID_H
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"
#include "hal/spi_types.h"
#include "sdkconfig.h"

struct {
  spi_device_handle_t RFID_handle;
  spi_device_handle_t LoRa_handle;
} spi_handles;
void init_SPI_bus();
#endif
