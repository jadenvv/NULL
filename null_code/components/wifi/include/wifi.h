#ifndef WIFI_H
#define WIFI_H
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define TASK_STACK_SIZE 200

esp_err_t init_wifi();
esp_err_t find_conn_AP();

#endif
