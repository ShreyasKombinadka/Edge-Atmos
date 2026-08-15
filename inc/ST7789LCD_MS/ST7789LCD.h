#ifndef ST7789LCD_H
#define ST7789LCD_H

#include <stdint.h>

void st7789lcd_init(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT, uint8_t ROTATION); // TFT(ST7789) LCD display initialisation
void st7789lcd_print(uint8_t *TEXT, uint16_t ROW_START_ADDR, uint16_t COL_START_ADDR, uint16_t TEXT_COLOR, uint16_t BG_COLOR, uint8_t TEXT_SIZE,
                     uint8_t ROTATION, uint8_t TEXT_ROW_PIXEL_COUNT, uint8_t TEXT_COL_PIXEL_COUNT, uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT);           // Display write function
void st7789lcd_clear(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint16_t BG_COLOR, int8_t ROTATION, uint16_t ROW_END, uint16_t COL_END);                 // Display clear
void st7789lcd_setup(uint8_t DC, uint8_t DC_PORT, uint8_t ROTATION);                                                                                                    // Sets display orientation and RGB settings
void st7789lcd_setsize(uint8_t DC, uint8_t DC_PORT, uint8_t ROTATION, uint8_t FULL_SCREEN, uint16_t ROW_START, uint16_t COL_START, uint16_t ROW_END, uint16_t COL_END); // Sets pixel grid size

#endif