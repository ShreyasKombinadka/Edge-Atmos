#include "AHT10.h"
#include "../i2c/i2c.h"
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

    *temp = (((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) * 200) / 1048576.0) - 50;
    *humi = (((data[1] << 12) | (data[2] << 4) | ((data[3] & 0xF0) >> 4)) * 100) / 1048576.0;
}
