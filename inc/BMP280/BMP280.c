#include "BMP280.h"
#include "../i2c/i2c.h"
#include <stdint.h>

void bmp280_init(uint8_t *FACTORY_CALIBRATION_DATA)
{
    i2c1_wake(0x76);

    i2c1_w1byte(0xE0);
    i2c1_w1byte(0xB6);

    i2c1_w1byte(0xF5);
    i2c1_w1byte(0x1C);

    i2c1_w1byte(0xF4);
    i2c1_w1byte(0x2F);
    for (volatile int i = 0; i < 120; i++)
        for (volatile int j = 0; j < 120; j++)
            ;

    i2c1_w1byte(0x88);
    i2c1_rsnbyte(0x76, FACTORY_CALIBRATION_DATA, 24);

    i2c1_stop();
}

void bmp280_read(uint8_t *FACTORY_CALIBRATION_DATA, float *pres, float *temp)
{
    uint8_t data[6];

    i2c1_wake(0x76);
    i2c1_w1byte(0xF7);
    i2c1_rsnbyte(0x76, data, 6);

    uint16_t TSFB = (FACTORY_CALIBRATION_DATA[1] << 8) | FACTORY_CALIBRATION_DATA[0]; // Temperature Scale Factor Baseline
    int16_t TFOC = (FACTORY_CALIBRATION_DATA[3] << 8) | FACTORY_CALIBRATION_DATA[2];  // Temperature First-order Coefficient
    int16_t TSOC = (FACTORY_CALIBRATION_DATA[5] << 8) | FACTORY_CALIBRATION_DATA[4];  // Temperature Second-order Coefficient
    uint16_t PSFB = (FACTORY_CALIBRATION_DATA[7] << 8) | FACTORY_CALIBRATION_DATA[6]; // Pressure Scale Factor Baseline

    // Pressure Linearity Compensation Parameters
    int16_t PLCP[8];
    uint8_t index = 8;
    for (volatile int i = 0; i < 8; i++)
    {
        PLCP[i] = (FACTORY_CALIBRATION_DATA[index + 1] << 8) | FACTORY_CALIBRATION_DATA[index];
        index += 2;
    }

    int32_t raw_temp = (data[3] << 12) | (data[4] << 4) | ((data[5] & ~0x0F) >> 4);
    int32_t raw_pres = (data[0] << 12) | (data[1] << 4) | ((data[2] & ~0x0F) >> 4);

    // Below are weird BMP280 calcuations that i do not undersatnd at all so just told ai to write it for me and copy pasted it here...
    int32_t var1 = ((((raw_temp >> 3) - ((int32_t)TSFB << 1))) * (int32_t)TFOC) >> 11;
    int32_t var2 = (((((raw_temp >> 4) - (int32_t)TSFB) * ((raw_temp >> 4) - (int32_t)TSFB)) >> 12) * (int32_t)TSOC) >> 14;
    int32_t t_fine = var1 + var2;

    *temp = (float)t_fine / 5120.0f;

    int64_t var1_p, var2_p, p;
    var1_p = ((int64_t)t_fine) - 128000;
    var2_p = var1_p * var1_p * (int64_t)PLCP[4];
    var2_p = var2_p + ((var1_p * (int64_t)PLCP[3]) << 17);
    var2_p = var2_p + (((int64_t)PLCP[2]) << 35);
    var1_p = ((var1_p * var1_p * (int64_t)PLCP[1]) >> 8) + ((var1_p * (int64_t)PLCP[0]) << 12);
    var1_p = (((((int64_t)1) << 47) + var1_p) * (int64_t)PSFB) >> 33;
    if (var1_p != 0)
    {
        p = 1048576 - raw_pres;
        p = (((p << 31) - var2_p) * 3125) / var1_p;
        var1_p = ((int64_t)PLCP[7] * (p >> 13) * (p >> 13)) >> 25;
        var2_p = ((int64_t)PLCP[6] * p) >> 19;
        p = ((p + var1_p + var2_p) >> 8) + (((int64_t)PLCP[5]) << 4);

        *pres = (float)p / 25600.0f;
    }
    else
        *pres = 0;
}
