#ifndef WIFI_H
#define WIFI_H
#include "esp_timer.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define TASK_STACK_SIZE 200
static char *TAG = "WIFI";
static esp_timer_handle_t timer_handle;
esp_err_t init_wifi();
esp_err_t find_conn_AP();

#endif
