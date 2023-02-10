#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#define SENDERTAG_MAX_LEN   (64)

typedef struct _display_data_t {
    uint8_t row;
    uint8_t col;
    char data[16];
    uint8_t length;
    char senderTAG[SENDERTAG_MAX_LEN];
} display_data_t;

esp_err_t display_init();
esp_err_t display_write_buffer(uint8_t row, uint8_t col, char* data, uint8_t data_length, char* sender);
void display_clear_screen();

void display_handler(void* param);



#endif
