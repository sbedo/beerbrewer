#include <stdio.h>
#include "rom/ets_sys.h"
#include <esp_log.h>
#include "unistd.h"

#include "driver_I2C.h"
#include "HD44780.h"

static const char *TAG = "LCD";

static void hd44780_writeNibble(uint8_t nibble, uint8_t mode);
static void hd44780_writeByte(uint8_t data, uint8_t mode);
static void hd44780_pulseEnable(uint8_t nibble);

#define I2C_NUM I2C_NUM_0
#define SLAVE_ADDRESS_LCD 0x4E>>1 // change this according to ur setup
esp_err_t err;

void lcd_send_cmd (char cmd)
{
	esp_err_t err;
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0

    err = I2C_send_data(SLAVE_ADDRESS_LCD, data_t, 4);

	if (err != 0) ESP_LOGI(TAG, "Error no. %d in command", err);

}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	
    err = I2C_send_data(SLAVE_ADDRESS_LCD, data_t, 4);

	if (err!=0) ESP_LOGI(TAG, "Error in sending data");
}

void lcd_init (void)
{
    if(true != I2C_is_initialized()){
        ESP_LOGI(TAG, "I2C not initialized!");
    } else {
        // 4 bit initialisation
        usleep(50000);  // wait for >40ms
        lcd_send_cmd (0x30);
        usleep(4500);  // wait for >4.1ms
        lcd_send_cmd (0x30);
        usleep(200);  // wait for >100us
        lcd_send_cmd (0x30);
        usleep(200);
        lcd_send_cmd (0x20);  // 4bit mode
        usleep(200);

    // dislay initialisation
        lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
        usleep(1000);
        lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
        usleep(1000);
        lcd_send_cmd (0x01);  // clear display
        usleep(1000);
        usleep(1000);
        lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
        usleep(1000);
        lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
        usleep(2000);
    }
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
        case 2:
            col |= 0x90;
            break;
        case 3:
            col |= 0xD0;
            break;
    }

    lcd_send_cmd (col);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear (void)
{
	lcd_send_cmd (0x01);
    vTaskDelay(pdMS_TO_TICKS(5));
    
}