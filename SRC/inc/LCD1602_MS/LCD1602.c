#include "LCD1602.h"
#include "../I2C_MS/I2C1.h"
#include <stdint.h>

void lcd1602_init(void)
{
    i2c1_wake(LCD_ADDR);

    lcd1602_w1byte(0x33, 0);
    lcd1602_w1byte(0x32, 0);
    lcd1602_w1byte(0x28, 0);
    lcd1602_w1byte(0x0C, 0);
    lcd1602_w1byte(0x06, 0);
    lcd1602_w1byte(0x1, 0);

    i2c1_stop();
}

void lcd1602_w1byte(uint8_t data, uint8_t rs)
{
    i2c1_w1byte((data & ~0xF) | 0xC | rs);
    i2c1_w1byte((data & ~0xF) | 0x8 | rs);
    for (volatile int i = 0; i < 50; i++)
        ;
    i2c1_w1byte(((data << 4) & ~0xF) | 0xC | rs);
    i2c1_w1byte(((data << 4) & ~0xF) | 0x8 | rs);
    for (volatile int i = 0; i < 1500; i++)
        ;
}

void lcd1602_wwscmd(uint8_t cmd)
{
    i2c1_wake(LCD_ADDR);

    lcd1602_w1byte(cmd, 0);

    i2c1_stop();
}

void lcd1602_char(uint8_t data)
{
    i2c1_wake(LCD_ADDR);

    lcd1602_w1byte(data, 1);

    i2c1_stop();
}

void lcd1602_print(uint8_t *char_addr)
{
    i2c1_wake(LCD_ADDR);

    uint8_t count = 0;
    while (char_addr[count] != '\0')
    {
        lcd1602_w1byte(char_addr[count], 1);

        count++;
    }

    i2c1_stop();
}

void lcd1602_debug(uint8_t *char_addr)
{
    i2c1_wake(LCD_ADDR);

    lcd1602_w1byte(0x1, 0);

    uint8_t count = 0;
    while (char_addr[count] != '\0')
    {
        lcd1602_w1byte(char_addr[count], 1);

        count++;
    }

    for (volatile int d = 0; d < 1000000; d++)
        ;
    lcd1602_w1byte(0x1, 0);

    i2c1_stop();
}

void lcd1602_clear(void)
{
    i2c1_wake(LCD_ADDR);
    lcd1602_w1byte(0x01, 0);
    i2c1_stop();
}

void lcd1602_row2(void)
{
    i2c1_wake(LCD_ADDR);
    lcd1602_w1byte(0xC0, 0);
    i2c1_stop();
}

void lcd1602_space(void)
{
    i2c1_wake(LCD_ADDR);
    lcd1602_w1byte(0x20, 0);
    i2c1_stop();
}