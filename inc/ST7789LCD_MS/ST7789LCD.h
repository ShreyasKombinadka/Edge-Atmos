#ifndef ST7789LCD_H
#define ST7789LCD_H

#include <stdint.h>

void st7789lcd_init(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT); // TFT(ST7789) LCD display without touch initialisation
void st7789lcd_memtest(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint16_t FILL_COLORE);                                      // Memory test by filling color

#endif