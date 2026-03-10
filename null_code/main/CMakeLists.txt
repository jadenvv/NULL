idf_component_register(SRCS "null.c" "button.c" "i2c.c" 
                    INCLUDE_DIRS "."
	    	REQUIRES esp_driver_i2c
		REQUIRES esp_lcd
		REQUIRES esp_wifi
		REQUIRES esp_driver_gpio
		REQUIRES esp_timer)
