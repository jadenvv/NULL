#include "wifi.h"
#include "freertos/idf_additions.h"
static char *TAG_WIFI = "WIFI";
static esp_timer_handle_t timer_handle;
static inline void scanning(void *args) {
  vTaskSuspend(NULL);
  esp_wifi_scan_start((wifi_scan_config_t *)args, false);
  return;
}
static inline void timer_callback(void *args) {
  ESP_LOGI(TAG_WIFI, "killing scanning ");
  esp_wifi_scan_stop();
  vTaskDelete(args);
}
esp_err_t init_wifi() {
  esp_err_t error = ESP_OK;
  wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
  error = esp_wifi_init(&config);
  error = esp_wifi_set_mode(WIFI_MODE_STA);
  error = esp_wifi_start();

  return error;
}
esp_err_t find_conn_AP() {
  esp_err_t err = ESP_OK;
  StackType_t stack[200];
  StaticTask_t task_buffer;
  wifi_scan_config_t config = WIFI_SCAN_PARAMS_DEFAULT_CONFIG();
  TaskHandle_t handler_task =
      xTaskCreateStatic(scanning, "scanning", TASK_STACK_SIZE, (void *)&config,
                        2, stack, &task_buffer);
  esp_timer_create_args_t timer_config = {.callback = timer_callback,
                                          .arg = (void *)handler_task,
                                          .name = "AP_TIMER",
                                          .skip_unhandled_events = false

  };
  esp_timer_create(&timer_config, &timer_handle);
  esp_timer_start_once(timer_handle, 10000000);
  vTaskResume(handler_task);

  return err;
}
esp_err_t ap_connect() {
  uint16_t num_APs;
  size_t i = 0;
  esp_wifi_scan_get_ap_num(&num_APs);
  wifi_ap_record_t *records =
      heap_caps_malloc(sizeof(wifi_ap_record_t), MALLOC_CAP_INTERNAL);
  memcpy(records, 0, num_APs * sizeof(wifi_ap_record_t));
  for (; i < num_APs; i++) {
    if (strcmp((char *)records[i].ssid, CONFIG_SSID) == 0)
      break;
  }
  wifi_sta_config_t conf = {
      .bssid_set = true,
      .channel = records[i].primary,
      .scan_method = WIFI_CONNECT_AP_BY_SIGNAL,
  };
  strncpy((char *)conf.bssid, (char *)records[i].bssid, 6);
  strncpy((char *)conf.ssid, (char *)records[i].ssid, 33);
  strcpy((char *)conf.password, CONFIG_PASS);
  esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t *)&conf);
  esp_wifi_connect();
  // lwkirk we need to add event handling to see if i'm actually connected or no
  // lmao

  return ESP_OK;
}
