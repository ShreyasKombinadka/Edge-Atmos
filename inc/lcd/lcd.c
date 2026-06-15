#include "lcd.h"
#include "../i2c/i2c.h"
#include <stdint.h>

void lcd_init(uint8_t addr)
{
    i2c1_wake(addr);

    lcd_w1byte(0x33, 0);

    lcd_w1byte(0x32, 0);

    lcd_w1byte(0x28, 0);

    lcd_w1byte(0x0C, 0);

    lcd_w1byte(0x06, 0);

    lcd_w1byte(0x1, 0);

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

    for (volatile int i = 0; i < 300; i++)
        ;
}

void lcd_wwscmd(uint8_t addr, uint8_t cmd)
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

void lcd_wws8byte(uint8_t addr, uint64_t data, int n)
{
    i2c1_wake(addr);

    uint8_t data_temp;

    int count = n;
    while (count >= 0)
    {
        data_temp = (data >> (8 * count));

        lcd_w1byte(data_temp, 1);

        count--;
    }

    i2c1_stop();
}

void lcd_wwschar(uint8_t addr, uint8_t *char_addr)
{
    i2c1_wake(addr);

    uint8_t count = 0;
    while (char_addr[count] != '\0')
    {
        lcd_w1byte(char_addr[count], 1);

        count++;
    }

    i2c1_stop();
}

void lcd_debug(uint8_t addr, uint8_t *char_addr)
{
    i2c1_wake(addr);

    lcd_w1byte(0x1, 0);

    uint8_t count = 0;
    while (char_addr[count] != '\0')
    {
        lcd_w1byte(char_addr[count], 1);

        count++;
    }

    for (volatile int d = 0; d < 1000000; d++)
        ;
    lcd_w1byte(0x1, 0);

    i2c1_stop();
}

void lcd_clear(uint8_t addr)
{
    lcd_wwscmd(addr, 1);
}