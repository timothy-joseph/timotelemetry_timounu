idf_component_register(SRCS "rtc_max31331.c"
                    INCLUDE_DIRS "include"
		    REQUIRES driver can esp_timer)
