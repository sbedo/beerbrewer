# set(COMPONENT_ADD_INCLUDEDIRS include)
# set(COMPONENT_SRCDIRS ".")
# register_component()

idf_component_register(SRCS "driver_HD44780.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                    REQUIRES driver_I2C)