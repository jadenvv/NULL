#include "lora_rfid.h"
#include "esp_log.h"
static const char *SPI_TAG = "SPI";
spi_handles init_SPI_bus() {
  lr_handles = {0};
  spi_bus_config_t bus_config = {.mosi_io_num = CONFIG_SPI_MOSI,
                                 .miso_io_num = CONFIG_SPI_MISO,
                                 .sclk_io_num = CONFIG_SPI_CLK,
                                 .isr_cpu_id = ESP_INTR_CPU_AFFINITY_0,
                                 .intr_flags = ESP_INTR_FLAG_NMI};
  ESP_ERROR_CHECK(spi_bus_initialize(SPI1_HOST, &bus_config, SPI_DMA_CH_AUTO));
  ESP_LOGI(SPI_TAG, "SPI bus intialized");
  // initializing RFID
  spi_device_handle_t *handles_tmp =
      heap_caps_calloc(2, sizeof(spi_device_handle_t), MALLOC_CAP_DEFAULT);
  lr_handles.RFID_handle = &handles_tmp[0];
  lr_handles.LoRa_handle = &handles_tmp[1];
  spi_device_interface_config_t config_RFID = {
      .command_bits = 0,
      .address_bits = 0, // pg 31 of trF datasheet
      .dummy_bits = 0,
      .mode = 1, // page 39 of ds
      .clock_source = SPI_CLK_SRC_XTAL,
      .spics_io_num = CONFIG_RFID_CS,
      .queue_size = 5, // this is lwkirk abitrarily picked haha
      //  Could add pre tranmission cb functiion for Dma or smth in the future
  };

  spi_bus_add_device(SPI1_HOST, &config_RFID, handles.RFID_handle);

  // start condition for IC
  addr_read(0x0C);
  addr_read(0x0D);
  send_cmd(0x03);
  send_cmd(0x00);

  // initializing LORA
  spi_device_interface_config_t config_LoRa = {

  };
  spi_bus_add_device(SPI1_HOST, &config_LoRa, handles.LoRa_handle);
  return handles;
}

// NOTE: the settings seem really arbitrary but just look at the TRF docs on
// page 23 &24
RFID_seq recv_RFID() {
  RFID_cond = RECV;
  uint8_t settings = 0x28;
  addr_write(0x01, &settings, 1);
  send_cmd(0x17); // initiate recieving refer to docs at pg 51
  return get_RFID();
}
void trans_RFID(RFID_seq seq) {
  RFID_cond = TRANS;
  uint8_t settings = 0x3C;
  addr_write(0x01, &settings, 1);
  send_cmd(0x0F); // clear FIFO
  send_cmd(0x11); // initiate transmiting

  heap_caps_free(seq->buffer);
  seq->buffer = NULL;
}

void init_RFID_loRa() {
  init_SPI_bus();
  init_RFID();
}
