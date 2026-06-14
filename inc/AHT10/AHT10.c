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
    for (volatile int i = 0; i < 1000; i++)
        for (volatile int j = 0; j < 2400; j++)
            ;

    i2c1_stop();
}

void aht10_read(uint8_t *temp, uint8_t *humi)
{
    static uint8_t data[6];

    i2c1_wake(0x38);

    i2c1_w1byte(0xAC);
    i2c1_w1byte(0x33);
    i2c1_w1byte(0x00);
    for (volatile int i = 0; i < 750; i++)
        ;

    i2c1_rsnbyte(0x38, data, 6);

    *temp = data[5] | data[4] | ((data[3] & ~0xF0) << 4);
}