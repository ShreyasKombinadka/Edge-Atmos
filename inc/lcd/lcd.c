#include "lcd.h"
#include "../i2c/i2c.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void lcd_w1byte(uint8_t addr, uint8_t data)
{
    I2C1_wake(addr);

    I2C1_w1byte(0x1);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;

    I2C1_w1byte((data & ~0xF) | 0xC);
    I2C1_w1byte((data & ~0xF) | 0x8);
    for (volatile int i = 0; i < 750; i++)
        ;
    I2C1_w1byte(((data << 4) & ~0xF) | 0xC);
    I2C1_w1byte(((data << 4) & ~0xF) | 0x8);
    for (volatile int i = 0; i < 750; i++)
        ;
}

void lcd_wnbyte(uint8_t addr, uint64_t data, int n)
{
    I2C1_wake(addr);

    I2C1_w1byte(0x1);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;

    uint8_t data_temp = data;

    int count = 0;
    while (count <= n - 1)
    {
        data_temp = (data >> (8 * count));

        I2C1_w1byte((data_temp & ~0x0F) | 0xC);
        I2C1_w1byte((data_temp & ~0x0F) | 0x8);
        for (volatile int i = 0; i < 750; i++)
            ;
        I2C1_w1byte(((data_temp << 4) & ~0x0F) | 0xC);
        I2C1_w1byte(((data_temp << 4) & ~0x0F) | 0x8);
        for (volatile int i = 0; i < 750; i++)
            ;

        count++;
    }
}