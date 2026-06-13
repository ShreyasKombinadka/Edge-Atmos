#include "../i2c/i2c.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void aht10_init(void)
{
    if (!(I2C1->CR1 & 1))
        i2c1_init();

    i2c1_wake(0x38);

    i2c1_w1byte(0xE1);
    i2c1_w1byte(0x08);
    i2c1_w1byte(0x00);
    for (volatile int i = 0; i < 750; i++)
        ;
}

uint64_t aht10_read(void)
{
    i2c1_w1byte(0xAC);
    i2c1_w1byte(0x33);
    i2c1_w1byte(0x00);
    for (volatile int i = 0; i < 750; i++)
        ;

    return i2c1_rsnbyte(0x38, 6);
}