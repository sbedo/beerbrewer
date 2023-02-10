#pragma once


// LCD module defines
#define hd44780_LINEONE             0x00        // start of line 1
#define hd44780_LINETWO             0x40        // start of line 2
#define hd44780_LINETHREE           0x14        // start of line 3
#define hd44780_LINEFOUR            0x54        // start of line 4

#define hd44780_BACKLIGHT           0x08
#define hd44780_ENABLE              0x04               
#define hd44780_COMMAND             0x00
#define hd44780_WRITE               0x01

#define hd44780_SET_DDRAM_ADDR      0x80
#define hd44780_READ_BF             0x40

// LCD instructions
#define hd44780_CLEAR               0x01        // replace all characters with ASCII 'space'
#define hd44780_HOME                0x02        // return cursor to first position on first line
#define hd44780_ENTRY_MODE          0x06        // shift cursor from left to right on read/write
#define hd44780_DISPLAY_OFF         0x08        // turn display off
#define hd44780_DISPLAY_ON          0x0C        // display on, cursor off, don't blink character
#define hd44780_FUNCTION_RESET      0x30        // reset the hd44780
#define hd44780_FUNCTION_SET_4BIT   0x28        // 4-bit data, 2-line display, 5 x 7 font
#define hd44780_SET_CURSOR          0x80        // set cursor position

#define SLAVE_ADDRESS_LCD           0x4E>>1 // change this according to ur setup

#define hd44780_rows                4
#define hd44780_cols                16

void lcd_init (void);
void lcd_send_cmd (char cmd);
void lcd_put_cur(int row, int col);
void lcd_send_string (char *str);
void lcd_clear (void);
void lcd_send_data (char data);
