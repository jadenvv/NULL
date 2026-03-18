#include "wifi.h"
esp_err_t init_wifi()
{
	esp_err_t error = ESP_OK; 
	wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
	error = esp_wifi_init(&config); 
	error = esp_wifi_set_mode(WIFI_MODE_STA); 
	error =esp_wifi_start();
	return error; 
}
esp_err_t find_conn_AP()
{
	esp_wifi_scan_config_t config = ;

} 
