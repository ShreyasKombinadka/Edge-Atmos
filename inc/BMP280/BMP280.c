#include "BMP280.h"
#include "../i2c/i2c.h"
#include <stdint.h>

void bmp280_init(uint8_t *fact_cali)
{
    i2c1_wake(0x76);

    i2c1_w1byte(0xB6);
    i2c1_w1byte(0xE0);
    i2c1_w1byte(0xF4);
    i2c1_w1byte(0x2E);
    for (volatile int i = 0; i < 80; i++)
        for (volatile int j = 0; j < 200; j++)
            ;

    i2c1_w1byte(0x88);
    i2c1_rsnbyte(0x76, fact_cali, 24);

    i2c1_stop();
}

void bmp280_read(uint8_t *fact_cali, float *prea, float *temp)
{
    uint8_t data[6];

    i2c1_wake(0x38);

    i2c1_w1byte(0xF7);

    i2c1_rsnbyte(0x76, data, 6);

    uint16_t TSFB = (fact_cali[1] << 8) | fact_cali[0]; // Temperature Scale Factor Baseline
    int16_t TFOC = (fact_cali[3] << 8) | fact_cali[2];  // Temperature First-order Coefficient
    int16_t TSOC = (fact_cali[5] << 8) | fact_cali[4];  // Temperature Second-order Coefficient
    uint16_t PSFB = (fact_cali[7] << 8) | fact_cali[6]; // Pressure Scale Factor Baseline
    int16_t PLCP[8];                                    // Pressure Linearity Compensation Parameters
    uint8_t index = 8;
    for (volatile int i = 0; i < 8; i++)
    {
        PLCP[i] = (fact_cali[index + 1] << 8) | fact_cali[index];
        index += 2;
    }

    *temp = 0;
    *prea = 0;
}