#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/i2c.h"

#include "driver_i2c_base.h"

static SemaphoreHandle_t i2c_semaphore = NULL;

esp_err_t i2c_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    esp_err_t ret;
    
    do {
        // Don't initialize twice
        if( NULL != i2c_semaphore ) {
            ret = ESP_FAIL;
            break;
        } else {
            i2c_semaphore = xSemaphoreCreateBinary();
            if( NULL == i2c_semaphore ) {
                ret = ESP_FAIL;
                break;
            }
        }

        i2c_config_t conf = {
            .mode               = I2C_MODE_MASTER,
            .sda_io_num         = I2C_MASTER_SDA_IO,
            .sda_pullup_en      = GPIO_PULLUP_DISABLE,  // Hardware provides 4.7kΩ pullups
            .scl_io_num         = I2C_MASTER_SCL_IO,
            .scl_pullup_en      = GPIO_PULLUP_DISABLE,  // Hardware provides 4.7kΩ pullups
            .master.clk_speed   = I2C_MASTER_FREQ_HZ,
        };

        ret = i2c_param_config(i2c_master_port, &conf);

        if(ret != ESP_OK)
            break;
        
        ret = i2c_driver_install(i2c_master_port, conf.mode,
                        I2C_MASTER_RX_BUF_LEN,
                        I2C_MASTER_TX_BUF_LEN, 0);

    } while(0);
    

    return ret;
}


esp_err_t i2c_write_data(uint32_t i2c_peripheral, uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    
    
    esp_err_t ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(i2c_peripheral, dev_addr, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

esp_err_t i2c_read_data(uint32_t i2c_peripheral, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
   return i2c_master_write_read_device(i2c_peripheral, dev_addr, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}
