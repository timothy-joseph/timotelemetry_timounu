idf_component_register(SRCS "sim7600x_h.c"
                    INCLUDE_DIRS "include"
		    REQUIRES driver rtc_max31331 logging esp_timer)
