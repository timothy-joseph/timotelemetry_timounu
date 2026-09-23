idf_component_register(SRCS "microsd.c"
                    INCLUDE_DIRS "include"
		    REQUIRES driver fatfs rtc_max31331 sim7600x_h)
