#include "AHT10.h"
#include "../i2c/i2c.h"
#include "../ascii/ascii.h"
#include "../lcd/lcd.h"
#include <stdint.h>

void aht10_init(void)
{
    i2c1_wake(0x38);

    i2c1_w1byte(0xBE);
    i2c1_w1byte(0x08);
    i2c1_w1byte(0x00);
    for (volatile int i = 0; i < 120; i++)
        for (volatile int j = 0; j < 120; j++)
            ;

    i2c1_stop();

    for (volatile int i = 0; i < 500; i++)
        ;
}

void aht10_read(float *temp, float *humi)
{
    uint8_t data[6];

    i2c1_wake(0x38);

    i2c1_w1byte(0xAC);
    i2c1_w1byte(0x33);
    i2c1_w1byte(0x00);
    for (volatile int i = 0; i < 230; i++)
        for (volatile int j = 0; j < 230; j++)
            ;
    ;

    i2c1_rsnbyte(0x38, data, 6);

    uint8_t arr0[6];
    num_uint_ascii(data[0], arr0);
    uint8_t arr1[6];
    num_uint_ascii(data[1], arr1);
    uint8_t arr2[6];
    num_uint_ascii(data[2], arr2);
    uint8_t arr3[6];
    num_uint_ascii(data[3], arr3);
    uint8_t arr4[6];
    num_uint_ascii(data[4], arr4);
    uint8_t arr5[6];
    num_uint_ascii(data[5], arr5);

    lcd_clear(0x27);
    lcd_print(0x27, arr0);
    lcd_char(0x27, 0x20);
    lcd_print(0x27, arr1);
    lcd_char(0x27, 0x20);
    lcd_print(0x27, arr2);
    lcd_char(0x27, 0x20);
    lcd_print(0x27, arr3);
    lcd_row2(0x27);
    lcd_char(0x27, 0x20);
    lcd_print(0x27, arr4);
    lcd_char(0x27, 0x20);
    lcd_print(0x27, arr5);
    for (volatile int i = 0; i < 1000; i++)
        for (volatile int j = 0; j < 10000; j++)
            ;
    ;

    *temp = (((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) * 200) / 1048576.0) - 50;
    *humi = (((data[1] << 12) | (data[2] << 4) | ((data[3] & 0xF0) >> 4)) * 100) / 1048576.0;
}
