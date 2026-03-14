idf_component_register(SRCS "i2c.c"
                    INCLUDE_DIRS "include"
		    REQUIRES esp_driver_i2c
		    REQUIRES esp_lcd
		    REQUIRES utility) 
