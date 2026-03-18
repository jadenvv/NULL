idf_component_register(SRCS "utility.c"
                    INCLUDE_DIRS "include"
		    REQUIRES i2c
		    REQUIRES esp_timer 
		    REQUIRES lvgl__lvgl 
		    REQUIRES wifi)
