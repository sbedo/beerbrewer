#ifndef I2C_MASTER
#define I2C_MASTER


#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

// LCD1602
#define LCD_NUM_ROWS               (2)
#define LCD_NUM_COLUMNS            (32)
#define LCD_NUM_VISIBLE_COLUMNS    (16)

#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_TX_BUF_LEN   (0)       //disabled
#define I2C_MASTER_RX_BUF_LEN   (0)       //disabled
#define I2C_MASTER_FREQ_HZ      (100000)
#define I2C_MASTER_SDA_IO       (23)
#define I2C_MASTER_SCL_IO       (22)

#define I2C_MASTER_TIMEOUT_MS   1000


esp_err_t i2c_init(void);
esp_err_t i2c_write_data(uint32_t i2c_peripheral, uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
esp_err_t i2c_read_data(uint32_t i2c_peripheral, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);

#endif