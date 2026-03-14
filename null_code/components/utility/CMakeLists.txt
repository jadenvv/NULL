idf_component_register(SRCS "utility.c"
                    INCLUDE_DIRS "include"
		    REQUIRES i2c
		    REQUIRES esp_timer )
