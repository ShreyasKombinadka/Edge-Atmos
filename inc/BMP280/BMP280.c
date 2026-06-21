#include "BMP280.h"
#include "../i2c/i2c.h"
#include "../ascii/ascii.h"
#include "../lcd/lcd.h"
#include <stdint.h>

void bmp280_init(uint8_t *FACTORY_CALIBRATION_DATA)
{
    i2c1_wake(0x76);

    i2c1_w1byte(0xE0);
    i2c1_w1byte(0xB6);
    i2c1_stop();
    for (volatile int i = 0; i < 80; i++)
        for (volatile int j = 0; j < 200; j++)
            ;

    i2c1_wake(0x76);
    i2c1_w1byte(0xF4);
    i2c1_w1byte(0x2E);
    i2c1_stop();
    for (volatile int i = 0; i < 80; i++)
        for (volatile int j = 0; j < 200; j++)
            ;

    i2c1_wake(0x76);
    i2c1_w1byte(0xF5);
    i2c1_w1byte(0x1C);
    i2c1_stop();
    for (volatile int i = 0; i < 80; i++)
        for (volatile int j = 0; j < 200; j++)
            ;

    i2c1_wake(0x76);
    i2c1_w1byte(0xD0);
    uint8_t id_data[2];
    i2c1_rsnbyte(0x76, id_data, 2);
    uint8_t id_arr0[6];
    num_uint_ascii(id_data[0], id_arr0);
    uint8_t id_arr1[6];
    num_uint_ascii(id_data[1], id_arr1);
    lcd_clear(0x27);
    lcd_print(0x27, id_arr0);
    lcd_char(0x27, 0x20);
    lcd_print(0x27, id_arr1);
    for (volatile int i = 0; i < 1000; i++)
        for (volatile int j = 0; j < 1000; j++)
            ;
    ;

    i2c1_wake(0x76);

    i2c1_w1byte(0x88);
    i2c1_rsnbyte(0x76, FACTORY_CALIBRATION_DATA, 24);

    i2c1_stop();

    uint8_t arr0[6];
    num_uint_ascii(FACTORY_CALIBRATION_DATA[0], arr0);
    uint8_t arr1[6];
    num_uint_ascii(FACTORY_CALIBRATION_DATA[1], arr1);
    uint8_t arr2[6];
    num_uint_ascii(FACTORY_CALIBRATION_DATA[2], arr2);
    uint8_t arr3[6];
    num_uint_ascii(FACTORY_CALIBRATION_DATA[3], arr3);
    uint8_t arr4[6];
    num_uint_ascii(FACTORY_CALIBRATION_DATA[4], arr4);
    uint8_t arr5[6];
    num_uint_ascii(FACTORY_CALIBRATION_DATA[5], arr5);

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
        for (volatile int j = 0; j < 1000; j++)
            ;
    ;
}

void bmp280_read(uint8_t *FACTORY_CALIBRATION_DATA, uint32_t *pres, float *temp)
{
    uint8_t data[6];

    i2c1_wake(0x76);

    i2c1_w1byte(0xF7);

    i2c1_rsnbyte(0x76, data, 6);

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
        for (volatile int j = 0; j < 1000; j++)
            ;
    ;

    uint16_t TSFB = (FACTORY_CALIBRATION_DATA[1] << 8) | FACTORY_CALIBRATION_DATA[0]; // Temperature Scale Factor Baseline
    int16_t TFOC = (FACTORY_CALIBRATION_DATA[3] << 8) | FACTORY_CALIBRATION_DATA[2];  // Temperature First-order Coefficient
    int16_t TSOC = (FACTORY_CALIBRATION_DATA[5] << 8) | FACTORY_CALIBRATION_DATA[4];  // Temperature Second-order Coefficient
    uint16_t PSFB = (FACTORY_CALIBRATION_DATA[7] << 8) | FACTORY_CALIBRATION_DATA[6]; // Pressure Scale Factor Baseline

    // Pressure Linearity Compensation Parameters
    uint16_t PLCP0 = (FACTORY_CALIBRATION_DATA[7] << 8) | FACTORY_CALIBRATION_DATA[6];
    int16_t PLCP[8];
    uint8_t index = 8;
    for (volatile int i = 0; i < 8; i++)
    {
        PLCP[i] = (FACTORY_CALIBRATION_DATA[index + 1] << 8) |
                  FACTORY_CALIBRATION_DATA[index];
        index += 2;
    }

    int32_t raw_temp = (data[3] << 12) | (data[4] << 4) | ((data[5] & ~0x0F) >> 4);
    int32_t raw_pres = (data[0] << 12) | (data[1] << 4) | ((data[2] & ~0x0F) >> 4);

    int32_t var1 = ((((raw_temp >> 3) - ((int32_t)TSFB << 1))) * (int32_t)TFOC) >> 11;
    int32_t var2 = (((((raw_temp >> 4) - (int32_t)TSFB) * ((raw_temp >> 4) - (int32_t)TSFB)) >> 12) * (int32_t)TSOC) >> 14;
    int32_t t_fine = var1 + var2;

    *temp = (float)t_fine / 5120.0f;

    var1 = (t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t)PLCP[1];
    var2 = var2 + ((var1 * (int32_t)PLCP0) << 1);
    var2 = (var2 >> 2) + ((int32_t)PSFB << 16);
    var1 = (((int32_t)PLCP[4] * ((var1 >> 2) * (var1 >> 2) >> 11)) >> 2) + (((int32_t)PLCP[3] * var1) >> 4);
    var1 = var1 + ((int32_t)PLCP[2] << 13);
    int32_t p_raw = 1048576 - raw_pres;
    if (var1 != 0)
    {
        p_raw = (((p_raw << 15) - var2) * 3125) / var1;
        var1 = ((int32_t)PLCP[7] * (p_raw >> 13) * (p_raw >> 13)) >> 25;
        var2 = ((int32_t)PLCP[6] * p_raw) >> 19;
        *pres = (uint32_t)(((p_raw + var1 + var2) >> 8) + ((int32_t)PLCP[5] << 4));
    }
    else
        *pres = 0;
}