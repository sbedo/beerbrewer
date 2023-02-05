#include "driver_I2C.h"
#include "HD44780.h"

static char tag[] = "LCD Driver";
static uint8_t LCD_addr;
static uint8_t LCD_cols;
static uint8_t LCD_rows;

static void LCD_writeNibble(uint8_t nibble, uint8_t mode);
static void LCD_writeByte(uint8_t data, uint8_t mode);
static void LCD_pulseEnable(uint8_t nibble);


void LCD_init(uint8_t addr, uint8_t cols, uint8_t rows)
{
    LCD_addr = addr;
    LCD_cols = cols;
    LCD_rows = rows;
    vTaskDelay(100 / portTICK_PERIOD_MS);                                 // Initial 40 mSec delay

    // Reset the LCD controller
    LCD_writeNibble(LCD_FUNCTION_RESET, LCD_COMMAND);                   // First part of reset sequence
    vTaskDelay(10 / portTICK_PERIOD_MS);                                  // 4.1 mS delay (min)
    LCD_writeNibble(LCD_FUNCTION_RESET, LCD_COMMAND);                   // second part of reset sequence
    ets_delay_us(200);                                                  // 100 uS delay (min)
    LCD_writeNibble(LCD_FUNCTION_RESET, LCD_COMMAND);                   // Third time's a charm
    LCD_writeNibble(LCD_FUNCTION_SET_4BIT, LCD_COMMAND);                // Activate 4-bit mode
    ets_delay_us(80);                                                   // 40 uS delay (min)

    // --- Busy flag now available ---
    // Function Set instruction
    LCD_writeByte(LCD_FUNCTION_SET_4BIT, LCD_COMMAND);                  // Set mode, lines, and font
    ets_delay_us(80); 

    // Clear Display instruction
    LCD_writeByte(LCD_CLEAR, LCD_COMMAND);                              // clear display RAM
    vTaskDelay(2 / portTICK_PERIOD_MS);                                   // Clearing memory takes a bit longer
    
    // Entry Mode Set instruction
    LCD_writeByte(LCD_ENTRY_MODE, LCD_COMMAND);                         // Set desired shift characteristics
    ets_delay_us(80); 

    LCD_writeByte(LCD_DISPLAY_ON, LCD_COMMAND);                         // Ensure LCD is set to on
}

void LCD_setCursor(uint8_t col, uint8_t row)
{
    if (row > LCD_rows - 1) {
        ESP_LOGE(tag, "Cannot write to row %d. Please select a row in the range (0, %d)", row, LCD_rows-1);
        row = LCD_rows - 1;
    }
    uint8_t row_offsets[] = {LCD_LINEONE, LCD_LINETWO, LCD_LINETHREE, LCD_LINEFOUR};
    LCD_writeByte(LCD_SET_DDRAM_ADDR | (col + row_offsets[row]), LCD_COMMAND);
}

void LCD_writeChar(char c)
{
    LCD_writeByte(c, LCD_WRITE);                                        // Write data to DDRAM
}

void LCD_writeStr(char* str)
{
    while (*str) {
        LCD_writeChar(*str++);
    }
}

void LCD_home(void)
{
    LCD_writeByte(LCD_HOME, LCD_COMMAND);
    vTaskDelay(2 / portTICK_PERIOD_MS);                                   // This command takes a while to complete
}

void LCD_clearScreen(void)
{
    LCD_writeByte(LCD_CLEAR, LCD_COMMAND);
    vTaskDelay(2 / portTICK_PERIOD_MS);                                   // This command takes a while to complete
}

static void LCD_writeNibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    I2C_send_byte(LCD_addr, data);

    LCD_pulseEnable(data);                                              // Clock data into LCD
}

static void LCD_writeByte(uint8_t data, uint8_t mode)
{
    LCD_writeNibble(data & 0xF0, mode);
    LCD_writeNibble((data << 4) & 0xF0, mode);
}

static void LCD_pulseEnable(uint8_t data)
{
    I2C_send_byte(LCD_addr, data | LCD_ENABLE);

    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // ESP_ERROR_CHECK(i2c_master_start(cmd));
    // ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (LCD_addr << 1) | I2C_MASTER_WRITE, 1));
    // ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data | LCD_ENABLE, 1));
    // ESP_ERROR_CHECK(i2c_master_stop(cmd));
    // ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
    // i2c_cmd_link_delete(cmd);  
    ets_delay_us(1);

    I2C_send_byte(LCD_addr, (data & ~LCD_ENABLE));

    // cmd = i2c_cmd_link_create();
    // ESP_ERROR_CHECK(i2c_master_start(cmd));
    // ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (LCD_addr << 1) | I2C_MASTER_WRITE, 1));
    // ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (data & ~LCD_ENABLE), 1));
    // ESP_ERROR_CHECK(i2c_master_stop(cmd));
    // ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
    // i2c_cmd_link_delete(cmd);
    ets_delay_us(500);
}