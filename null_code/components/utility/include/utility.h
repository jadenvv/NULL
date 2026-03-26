#ifndef UTILITY_H
#define UTILITY_H
#include "dtypes.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "i2c.h"
#include "lvgl.h"
#include "sdkconfig.h"
#include "wifi.h"
#include <string.h>

// ERRORS

#define ESP_ERR_WRONG_SCREEN 0x2000
esp_err_t configure_internal_state();
esp_err_t updating_internal(enum button_event event);
esp_err_t disconfigure_internal_state();
#endif
