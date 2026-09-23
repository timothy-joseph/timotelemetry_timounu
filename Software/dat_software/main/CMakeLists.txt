idf_component_register(SRCS "dat_software.c"
                    INCLUDE_DIRS "."
		    REQUIRES driver sim7600x_h rtc_max31331 can microsd logging lora_driver vehicle_status)
