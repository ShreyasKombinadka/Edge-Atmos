#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_en(uint8_t GPIO_PORT);                                      // GPIO PORT CLK enable
void gpio_setup(uint8_t PIN, uint8_t PIN_PORT, uint8_t PIN_CNF_MODE); // Enable GPIO PORT and set PIN configuration and mode
void gpio_setreset(uint8_t PIN, uint8_t PIN_PORT, uint8_t SELECT);    // PIN Set(SELECT = 1) or Reset(SELECT = 0)

#endif