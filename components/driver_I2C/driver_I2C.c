#include <stdio.h>
#include "driver/i2c.h"
#include <esp_log.h>
#include "sdkconfig.h"
#include "rom/ets_sys.h"
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"

#include "driver_I2C.h"

static SemaphoreHandle_t xSemaphore_i2c = NULL;
static bool i2c_init_status = false;

static const char *I2C_TAG = "I2C";

esp_err_t I2C_init(void) {
    esp_err_t err = ESP_OK;
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode               = I2C_MODE_MASTER,
        .sda_io_num         = GPIO_NUM_21,
        .scl_io_num         = GPIO_NUM_22,
        .sda_pullup_en      = GPIO_PULLUP_DISABLE,
        .scl_pullup_en      = GPIO_PULLUP_DISABLE,
        .master.clk_speed   = I2C_MASTER_FREQ_HZ
    };

    do {
        xSemaphore_i2c = xSemaphoreCreateBinary();
        if(NULL == xSemaphore_i2c) {
            err = ESP_FAIL;
            ESP_LOGI(I2C_TAG, "Error - Can not create binary semaphore!");
            break;
        }
        xSemaphoreGive(xSemaphore_i2c);

        err = i2c_param_config(i2c_master_port, &conf);
        if(ESP_OK == err) {
            err = i2c_driver_install(i2c_master_port, conf.mode,
                            I2C_MASTER_RX_BUF_LEN,
                            I2C_MASTER_TX_BUF_LEN, 
                            0);
        } else {
            break;
        }

        i2c_init_status = true;
        
    } while (0);   

    return err;
}

bool I2C_is_initialized() {
    return i2c_init_status;
}

SemaphoreHandle_t* I2C_get_semaphore() {
    return &xSemaphore_i2c;
}


esp_err_t I2C_send_data(uint8_t address, uint8_t* data, uint8_t length) {
    esp_err_t err = ESP_OK;

    if(xSemaphoreTake(xSemaphore_i2c, 100)) {
        err = i2c_master_write_to_device(I2C_MASTER_NUM, address, data, length, 1000);
	    
        if (err != ESP_OK) {
            ESP_LOGI(I2C_TAG, "Error in sending data");
        }

        xSemaphoreGive(xSemaphore_i2c);
    } else {
        ESP_LOGI(I2C_TAG, "Error - Can not take binary semaphore!");
        err = ESP_FAIL;
    }

    return err;
}

