#include "../i2c/i2c.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void aht10_init(void)
{
    if (!(I2C1->CR1 & 1))
        I2C1_init();

    I2C1_wake(0x38);

    I2C1_w1byte(0xE1);
    I2C1_w1byte(0x08);
    I2C1_w1byte(0x00);
    for (volatile int i = 0; i < 750; i++)
        ;
}

uint64_t aht10_read(void)
{
    I2C1_w1byte(0xAC);
    I2C1_w1byte(0x33);
    I2C1_w1byte(0x00);
    for (volatile int i = 0; i < 750; i++)
        ;

    return I2C1_rsnbyte(0x38, 6);
}