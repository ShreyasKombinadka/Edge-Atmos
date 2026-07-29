#ifndef ST7789LCD_H
#define ST7789LCD_H

#include <stdint.h>

void st7789lcd_init(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT); // TFT(ST7789) LCD display without touch initialisation
void st7789lcd_fillcolor(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint16_t FILL_COLOR);                                     // Fill display
void st7789lcd_disptest(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT);                                                           // Display test

void st7789lcd_print(uint8_t *TEXT, uint16_t X_START_ADDR, uint16_t Y_START_ADDR, uint16_t TEXT_COLOR, uint16_t BG_COLOR, uint8_t TEXT_SIZE,
                     uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT); // Display write function

#endif