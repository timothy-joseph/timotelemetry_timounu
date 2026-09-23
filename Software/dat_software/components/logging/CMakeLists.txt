idf_component_register(SRCS "logging.c"
                    INCLUDE_DIRS "include"
		    REQUIRES microsd driver rtc_max31331 sim7600x_h lora_driver
		    )
