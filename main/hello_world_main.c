#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"

#include "driver_I2C.h"
#include "HD44780.h"

static const char *LCD_TAG = "LCD";
static const char *I2C_TAG = "I2C";

void hd44780_Handler(void* param)
{
    char buffer[10];
    float num = 12.34;
    char numbuffer[20];
    
    lcd_init();

    while (true) {
        lcd_clear();
        lcd_clear();

        sprintf(buffer, "val=%.2f", num);
        lcd_put_cur(0, 0);
        lcd_send_string(buffer);

        lcd_put_cur(1, 0);
        lcd_send_string("from ESP32");

        for (int i = 1; i<= 10; i++) {
            lcd_put_cur(3, 0);
            sprintf(numbuffer, "%d", i);
            lcd_send_string(numbuffer);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
  
    }
}

void app_main(void)
{
    esp_err_t err = ESP_OK;

    err = I2C_init();
    if(err != ESP_OK)
    {
        ESP_LOGI(I2C_TAG, "I2C init failed", err);
    }

    //
    // Create Tasks
    //
    xTaskCreate(hd44780_Handler, "Demo Task", 2048, NULL, 5, NULL);

    while(1)
    {

    }

    
}