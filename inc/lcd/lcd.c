#include "lcd.h"
#include "../i2c/i2c.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void lcd_init(uint8_t addr)
{
    if (!(I2C1->CR1 & 1))
        i2c1_init();

    i2c1_wake(addr);

    lcd_w1byte(0x33, 0);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;
    lcd_w1byte(0x32, 0);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;
    lcd_w1byte(0x28, 0);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;
    lcd_w1byte(0x0C, 0);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;
    lcd_w1byte(0x06, 0);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;

    lcd_w1byte(0x1, 0);
    for (volatile int i = 0; i < 50; i++)
        for (volatile int j = 0; j < 600; j++)
            ;
    ;

    i2c1_stop();
}

void lcd_w1byte(uint8_t data, uint8_t rs)
{
    i2c1_w1byte((data & ~0xF) | 0xC | rs);
    i2c1_w1byte((data & ~0xF) | 0x8 | rs);
    for (volatile int i = 0; i < 750; i++)
        ;
    i2c1_w1byte(((data << 4) & ~0xF) | 0xC | rs);
    i2c1_w1byte(((data << 4) & ~0xF) | 0x8 | rs);
    for (volatile int i = 0; i < 750; i++)
        ;
}

void lcd_wwsc(uint8_t addr, uint8_t cmd)
{
    i2c1_wake(addr);

    lcd_w1byte(cmd, 0);

    i2c1_stop();
}

void lcd_wws1byte(uint8_t addr, uint8_t data)
{
    i2c1_wake(addr);

    lcd_w1byte(data, 1);

    i2c1_stop();
}

void lcd_wwsnbyte(uint8_t addr, uint64_t data, int n)
{
    i2c1_wake(addr);

    uint8_t data_temp = data;

    int count = 0;
    while (count <= n - 1)
    {
        data_temp = (data >> (8 * count));

        lcd_w1byte(data_temp, 1);

        count++;
    }

    i2c1_stop();
}