#include "lora_rfid.h"
void init_SPI_bus() {
  spi_bust_config_t bus_config = {.mosi_io_num = CONFIG_SPI_MOSI,
                                  .miso_io_num = CONFIG_SPI_MISO,
                                  .sclk_io_num = CONFIG_SPI_CLK,
                                  .isr_cpu_id = ESP_INTR_CPU_AFFINITY_0,
                                  .intr_flags = ESP_INTR_FLAG_NMI};
  ESP_ERROR_CHECK(spi_bus_intialize(SPI1_HOST, &bus_config, SPI_DMA_CH_ATUO));
  ESP_LOGI(SPI_TAG, "SPI bus intialized");
  // initializing RFID
  heap_caps_calloc(1, sizeof(spi_device_handle_t), MALLOC_CAP_DEFAULT);
  spi_device_interface_config_t config_RFID = {

  };
  spi_bus_add_device(SPI1_HOST, )
  // initializing LORA
}
