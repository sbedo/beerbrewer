#include <stdio.h>
#include <string.h>
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "display_controller.h"
#include "driver_hd44780.h"

static const char *DISPLAY_TAG = "display";

/* As the display is 4x16 character */
const uint8_t display_rows = hd44780_rows;
const uint8_t display_cols = hd44780_cols;

/* Define the queue parameters. */
#define QUEUE_LENGTH        (32)
#define QUEUE_ITEM_SIZE     sizeof( display_data_t )

static QueueHandle_t xQueueDisplayData;
static SemaphoreHandle_t xSemaphore_display = NULL;


esp_err_t display_init()
{
    esp_err_t err = ESP_OK;

    do {
        xSemaphore_display = xSemaphoreCreateBinary();
        if(NULL == xSemaphore_display) {
            err = ESP_FAIL;
            ESP_LOGI(DISPLAY_TAG, "Error - Can not create binary semaphore!");
            break;
        }
        xSemaphoreGive(xSemaphore_display);

        /* Init hd44780 lcd display */
        lcd_init();

        /* Init Queue */
        /* Create the queue, storing the returned handle in the xQueue variable. */
        xQueueDisplayData = xQueueCreate( QUEUE_LENGTH, sizeof( display_data_t ) );
        if( xQueueDisplayData == NULL )
        {
            ESP_LOGE(DISPLAY_TAG, "Queue creation was unsuccessful");
            /* The queue could not be created – do something. */
        } else {
            ESP_LOGI(DISPLAY_TAG, "Init: Queue creation was successful");
        }
        

    } while(0);

    return err;
}

esp_err_t display_write_buffer(uint8_t row, uint8_t col, char* data, uint8_t data_length, char* sender) {
    esp_err_t err = ESP_OK;
    display_data_t displayContext;

    do {
        if((row > display_rows) || (col > display_cols)) {
            err = ESP_FAIL;
            break;
        }

        displayContext.row = row;
        displayContext.col = col;
        strcpy(displayContext.data, data);
        displayContext.length = data_length;
        uint8_t sender_length = strlen(sender);
        sender_length = (sender_length > SENDERTAG_MAX_LEN) ? SENDERTAG_MAX_LEN : sender_length;

        memcpy(displayContext.senderTAG, sender, sender_length);

         ESP_LOGI(DISPLAY_TAG, "Write_buffer: row: %d col: %d data: %s", row, col, data);
        
        /* Send the message to the queue, waiting for 10 ticks for space to become
        available if the queue is already full. */
        if(xQueueDisplayData != NULL)
        {
            if( xQueueSendToBack( xQueueDisplayData, &displayContext, 10 ) != pdPASS )
            {
                /* Data could not be sent to the queue even after waiting 10 ticks. */
                err = ESP_FAIL;
                ESP_LOGI(DISPLAY_TAG, "%s", "Failed to write into queue");  
                break;
            } else {
                 ESP_LOGI(DISPLAY_TAG, "Data sent with queue");
            }
        } else {
            ESP_LOGI(DISPLAY_TAG, "Write_buffer: queue is NULL");
            break;
        }

    } while(0);
    
    return err;
}

void display_clear_screen() {
    lcd_clear();
}

/* Display handler task */
void display_handler(void* param) {
    display_data_t display_context_to_write;
    static int tmp_cnt = 0;
    
    // display_init();

    display_clear_screen();

    while(1) {
        
        if(xQueueDisplayData != NULL) {
            if(pdTRUE == xQueueReceive(xQueueDisplayData, &display_context_to_write, portMAX_DELAY ))
            {
                lcd_put_cur(display_context_to_write.row, display_context_to_write.col);               
                lcd_send_string(display_context_to_write.data);
            }
        } else {
            ESP_LOGE(DISPLAY_TAG, "xQueueDisplayData is NULL");
        }

         vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}