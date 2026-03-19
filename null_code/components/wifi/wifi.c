#include "wifi.h"
#include "freertos/idf_additions.h"
static inline void scanning(void *args) {
  esp_wifi_scan_start((wifi_scan_config_t *)args, false);
}
static inline void timer_callback(void *args) {
  ESP_LOGI(TAG, "killing scanning ");
  vTaskDelete(args);
}
esp_err_t init_wifi() {
  esp_err_t error = ESP_OK;
  wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
  error = esp_wifi_init(&config);
  error = esp_wifi_set_mode(WIFI_MODE_STA);
  error = esp_wifi_start();
  esp_timer_create_args_t timer_config = {.callback = timer_callback,
                                          .arg = (void *)1,
                                          .name = "AP_TIMER",
                                          .skip_unhandled_events = false

  };
  esp_timer_create(&timer_config, &timer_handle);
  return error;
}
esp_err_t find_conn_AP() {
  esp_err_t err = ESP_OK;
  StackType_t stack[200];
  StaticTask_t task_buffer;
  esp_wifi_scan_config_t config = WIFI_SCAN_PARAMS_DEFAULT_CONFIG();
  TaskHandle_t handler_task =
      xTaskCreateStatic(scanning, "scanning", TASK_STACK_SIZE, (void *)&config,
                        2, stack, &task_buffer);
}
