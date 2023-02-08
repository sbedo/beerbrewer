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

esp_err_t I2C_init(void)
{
    // xSemaphore_i2c = xSemaphoreCreateBinary();

    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode               = I2C_MODE_MASTER,
        .sda_io_num         = GPIO_NUM_21,
        .scl_io_num         = GPIO_NUM_22,
        .sda_pullup_en      = GPIO_PULLUP_DISABLE,
        .scl_pullup_en      = GPIO_PULLUP_DISABLE,
        .master.clk_speed   = I2C_MASTER_FREQ_HZ
    };
	i2c_param_config(i2c_master_port, &conf);
    
    i2c_driver_install(i2c_master_port, conf.mode,
                    I2C_MASTER_RX_BUF_LEN,
                    I2C_MASTER_TX_BUF_LEN, 
                    0);
    
    i2c_init_status = true;

    return ESP_OK;
}

bool I2C_is_initialized()
{
    return i2c_init_status;
}

SemaphoreHandle_t* I2C_get_semaphore()
{
    return &xSemaphore_i2c;
}


esp_err_t I2C_send_byte(uint8_t address, uint8_t data)
{
    // if(xSemaphoreTake(xSemaphore_i2c, portMAX_DELAY))
    // {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, 1));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data, ACK_EN));
        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000/portTICK_PERIOD_MS));
        i2c_cmd_link_delete(cmd);

        // xSemaphoreGive(xSemaphore_i2c);
    // }

    return ESP_OK;
}

