idf_component_register(SRCS "null.c"
                    INCLUDE_DIRS "."
	    	REQUIRES esp_driver_i2c
		REQUIRES esp_lcd)
