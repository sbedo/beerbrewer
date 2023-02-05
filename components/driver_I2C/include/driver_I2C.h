#ifndef DRIVER_I2C_H_
#define DRIVER_I2C_H_

#include <stdio.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
#include "sdkconfig.h"
#include "rom/ets_sys.h"

#define I2C_MASTER_NUM           I2C_NUM_0
#define I2C_MASTER_TX_BUF_LEN    0                     // disabled
#define I2C_MASTER_RX_BUF_LEN    0                     // disabled
#define I2C_MASTER_FREQ_HZ       100000
#define I2C_MASTER_SDA_IO        CONFIG_I2C_MASTER_SDA
#define I2C_MASTER_SCL_IO        CONFIG_I2C_MASTER_SCL
#define ACK_EN                   true

// Pin mappings
// P0 -> RS
// P1 -> RW
// P2 -> E
// P3 -> Backlight
// P4 -> D4
// P5 -> D5
// P6 -> D6
// P7 -> D7

esp_err_t I2C_init(void);
bool I2C_is_initialized();
SemaphoreHandle_t* I2C_get_semaphore();
esp_err_t I2C_send_byte(uint8_t address, uint8_t data);

#endif