idf_component_register(SRCS "display_controller.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver_HD44780
                    REQUIRES utilities_fifo
                    )
