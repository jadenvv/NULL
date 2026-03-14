idf_component_register(SRCS "GPIO.c"
                    INCLUDE_DIRS "include"
		    REQUIRES esp_driver_gpio
		    REQUIRES utility 
		    )
