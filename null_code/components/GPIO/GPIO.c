#include "GPIO.h"
static uint32_t GPIOS[] = {CONFIG_UP_BUTTON_GPIO, CONFIG_DOWN_BUTTON_GPIO,
                           CONFIG_SELECT_BUTTON_GPIO};
static const char *TAG = "GPIO: ";
static uint8_t *RFID_tag_emu = NULL;
static size_t size = 0;
static bool finished = false;
void gpio_handler_function(void *gpio_number) {
  int gpio = *(int *)gpio_number;
  updating_internal(gpio);
}
uint8_t addr_read(uint8_t address) {
  uint8_t base = ADDR_RFID(1) | address; // the register for IRQ status
  uint8_t buffer = 0;
  spi_transaction_t trans = {
      .length = 8,
      .rx_length = 0,
      .rx_buffer = &buffer,
      .tx_buffer = &base, // move to DMA?
  };
  spi_device_transmit(*lr_handles.RFID_handle, &trans);
  return buffer;
}
void addr_write(uint8_t address, uint8_t *buf, size_t sizeof_buf) {
  uint8_t write_to = ADDR_RFID(0) | address;
  spi_transaction_t trans = {
      .length =
          sizeof_buf * 8, // expecting sizeof_buf to be in bytes instead of bits
      .rx_length = 0,
      .rx_buffer = NULL,
      .tx_buffer = buf, // move to DMA?
  };
  spi_device_queue_trans(*lr_handles.RFID_handle, &trans, portMAX_DELAY);

  return;
}
void send_cmd(uint8_t cmd) {
  uint8_t send = CMD_RFID(false, cmd);
  spi_transaction_t trans = {
      .length = 8,
      .rx_length = 0,
      .rx_buffer = NULL,
      .tx_buffer = &send, // move to DMA?
  };
  spi_device_queue_trans(*lr_handles.RFID_handle, &trans, portMAX_DELAY);
}
void read_FIFO(uint8_t *buffer, size_t read_amount) {

  uint8_t base = ADDR_RFID(1) | 0x1F;
  // make sure read_amount is zero indexed
  for (size_t i = 0; i > read_amount; i++) {
    uint8_t tmp = 0;
    spi_transaction_t trans = {
        .length = 8,
        .rx_length = 0,
        .rx_buffer = &tmp,
        .tx_buffer = &base, // move to DMA?
    };
    spi_device_transmit(*lr_handles.RFID_handle, &trans);
    buffer[i] = tmp;
  }
  return 0;
}
void RFID_IRQ_handler(void *arg) {

  uint8_t flags = addr_read(0x0C);
  // this one is when a SOF is detected
  if (((flags >> 7) & 0xFF) == 1 && RFID_cond == RECV) {
    // then this is a end
    if (size != 0) {
      finished = true;
    }
  }
  // this is when FIFO has stuff in it
  if (((flags >> 6) & 0xFF) == 1 && RFID_cond == RECV) {
    uint8_t number_of_bytes = addr_read(0x1C) ^ 0x80; // don't need the 8th bit
    ESP_LOGI(TAG, "reading  %x", number_of_bytes);
    if (size == 0) {
      RFID_tag_emu = heap_caps_malloc(sizeof(uint8_t) * number_of_bytes,
                                      MALLOC_CAP_DEFAULT);
    } else {
      RFID_tag_emu =
          heap_caps_realloc(RFID_tag_emu, numbert_of_bytes, MALLOC_CAP_DEFAULT);
    }
    read_FIFO(RFID_tag_emu + size, number_of_bytes);
    size += number_of_bytes;
    // okay I may have to transpose the data since it is first in first out
  }
}

RFID_seq get_RFID() {
  while (finished == false)
    ;
  return {size, RFID_tag_emu};
}

esp_err_t init_buttons() {
  gpio_config_t gpio_cfg = {
      .pin_bit_mask = 0,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en =
          GPIO_PULLUP_ENABLE, // we need to pull up high since we don't have an
                              // external pull up resistor to +3v
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_NEGEDGE};
  for (size_t i = 0; i < 3; i++) {
    gpio_cfg.pin_bit_mask = BIT_MASK(GPIOS[i]);
    ESP_ERROR_CHECK(
        gpio_config(&gpio_cfg)); // Honestly this Line should work since in the
                                 // source code for gpio_config it doesn't need
                                 // an acutal copy of the struct
  }
  gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
  for (size_t i = 0; i < 3; i++) {
    gpio_isr_handler_add(GPIOS[i], gpio_handler_function, (void *)&GPIOS[i]);
  }
  return ESP_OK;
}
esp_err_t init_RFID() {

  gpio_pulldown_en(CONFIG_RFID_EN);
  gpio_set_direction(CONFIG_RFID_EN, GPIO_MODE_OUTPUT);
  gpio_set_level(CONFIG_RFID_EN, 1);
  gpio_config_t cfg_IRQ = {
      .pin_bit_mask = BIT_MASK(CONFIG_RFID_IRQ),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en =
          GPIO_PULLUP_ENABLE, // we need to pull up high since we don't have an
                              // external pull up resistor to +3v
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_NEG_EDGE};
  gpio_isr_handler_add(CONFIG_RFID_IRQ, RFID_IRQ_handler, (void *)1);
  return ESP_OK;
}
inline void soft_shutdown_RFID() {
  gpio_set_level(CONFIG_RFID_EN, 0);
  return;
}
