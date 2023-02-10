#include <stdio.h>
#include <string.h>

#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"
#include "esp_log.h"

#include "driver_I2C.h"
#include "display_controller.h"

static const char *MAIN_TAG = "main";

void app_main(void)
{
    esp_err_t err = ESP_OK;
    char testData[16] = {0};
    uint8_t tmp_cnt = 0;

    err = I2C_init();
    if(err != ESP_OK)
    {
        ESP_LOGI(MAIN_TAG, "I2C init failed", err);
    }

    display_init();
    

    //
    // Create Tasks
    //
    xTaskCreate(display_handler, "hd44780 display handler task", 2048, NULL, 5, NULL);



    while(1)
    {

        tmp_cnt++;
        sprintf(testData, "helloWorld: %d", tmp_cnt);

        display_write_buffer(1,0, testData, strlen(testData), MAIN_TAG);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }

    
}