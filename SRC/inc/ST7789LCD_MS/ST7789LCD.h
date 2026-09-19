#ifndef ST7789LCD_H
#define ST7789LCD_H

#include <stdint.h>

struct ST7789LCD
{
    uint8_t CS;
    uint8_t CS_PORT;
    uint8_t DC;
    uint8_t DC_PORT;
    uint8_t RST;
    uint8_t RST_PORT;
    uint8_t LED;
    uint8_t LED_PORT;

    uint16_t HEIGHT;
    uint16_t WIDTH;
    uint8_t ROTATION;

    uint16_t BG_COLOR[4];
    uint8_t TEXT_ROW_PIXEL_COUNT;
    uint8_t TEXT_COL_PIXEL_COUNT;

    uint16_t ROW_ADDR[10];
    uint16_t COL_ADDR[10];

    uint8_t BOX_NUM;
    uint16_t TEXT_BOX[40];
};

void st7789lcd_pinset(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT); // TFT(ST7789) LCD display pin connections
void st7789lcd_dispset(uint8_t ROTATION, uint16_t HEIGHT, uint16_t WIDTH);                                                                     // TFT(ST7789) LCD display parameters
void st7789lcd_init();                                                                                                                         // TFT(ST7789) LCD display initialisation
void st7789lcd_setup();                                                                                                                        // Sets display orientation and RGB settings
void st7789lcd_setsize(uint8_t FULL_SCREEN, uint16_t ROW_START, uint16_t COL_START, uint16_t ROW_END, uint16_t COL_END);                       // Sets pixel grid size
void st7789lcd_clearall(uint16_t BG_COLOR);                                                                                                    // Display clear
void st7789lcd_setbox(uint8_t BOX_NUM, uint16_t RS, uint16_t RE, uint16_t CS, uint16_t CE);                                                    // Saves box info in the text box array
void st7789lcd_inbox(uint8_t BOX_NUM);                                                                                                         // Required box call
void st7789lcd_fillbox(uint16_t COLOR);                                                                                                        // Fill box with single color
void st7789lcd_settext(uint8_t TEXT_ROW_PIXEL_COUNT, uint8_t TEXT_COL_PIXEL_COUNT);                                                            // Sets font pixel count
void st7789lcd_rowrst();                                                                                                                       // Resets row addr to 0
void st7789lcd_colrst();                                                                                                                       // Resets col addr to 0
void st7789lcd_addrst();                                                                                                                       // Resets row and col addr to 0
void st7789lcd_alladdrst();                                                                                                                    // Resets all row and col addr to 0
void st7789lcd_print(uint8_t *TEXT, uint16_t ROW_OFFSET, uint16_t COL_OFFSET, uint16_t TEXT_COLOR, uint8_t TEXT_SIZE);                         // Display write function

#endif