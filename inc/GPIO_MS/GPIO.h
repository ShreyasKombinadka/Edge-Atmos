#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_en(uint8_t GPIO_PORT);                                                     // GPIO PORT CLK enable
void gpio_setpin(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t PIN_CNF_MODE); // Enable GPIO PORT and set PIN configuration and mode

#endif