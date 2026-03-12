#ifndef UTILITY_H
#define UTILITY_H
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_intr_alloc"
#include "sdkconfig.h"
#include <string.h>
enum state {
	ARP_OPT,
	IR_EMU, 	
};
enum menu {
	MAIN,
};
struct current_state{
	enum state c_state; 
	enum menu c_menu; 
};
static lv_disp_t * display = NULL; 
static struct current_state* current = NULL;  
esp_err_t configure_internal_state(); 
esp_err_t disconfigure_internal_state(); 
#endif
