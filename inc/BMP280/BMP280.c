#include "BMP280.h"
#include "../i2c/i2c.h"
#include <stdint.h>

void bmp280_init(void)
{
    i2c1_wake(0x76);

    i2c1_w1byte(0xB6);
    i2c1_w1byte(0xE0);
    i2c1_w1byte(0xF4);
    i2c1_w1byte(0x2E);
    for (volatile int i = 0; i < 80; i++)
        for (volatile int j = 0; j < 200; j++)
            ;

    i2c1_stop();
}

void bmp280_read(float *prea, float *temp)
{
    uint8_t data[6];

    i2c1_wake(0x38);

    i2c1_w1byte(0xF7);

    i2c1_rsnbyte(0x38, data, 6);

    *prea = (((data[1] << 12) | (data[2] << 4) | (data[3] & 0x0F)) * 100) / 1048576.0;
    *temp = (((((data[3] & ~0xF0) << 16) | (data[4] << 8) | data[5]) * 200) / 1048576.0) - 50;
}