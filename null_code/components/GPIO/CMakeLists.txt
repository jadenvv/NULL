idf_component_register(SRCS "GPIO.c" "IR.c"
                    INCLUDE_DIRS "include"
      REQUIRES esp_driver_gpio
      REQUIRES utility
        REQUIRES esp_driver_spi
      REQUIRE esp_timer
      )
