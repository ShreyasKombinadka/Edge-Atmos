#include <stdint.h>
#include "./I2C_MS/I2C1.h"
#include "./SPI_MS/SPI1.h"
#include "./ASCII_MS/ASCII.h"
#include "./LCD1602_MS/LCD1602.h"
#include "./AHT10_MS/AHT10.h"
#include "./BMP280_MS/BMP280.h"
#include "./W25Q32_MS/W25Q32.h"

#define STM32F103xB
#include "./STM32F103_CMSIS/stm32f1xx.h"

int main(void)
{
    i2c1_init();
    spi1_init(0, 2);
    lcd1602_init();
    lcd1602_print("Edge Atmos");
    for (volatile int i = 0; i < 1000000; i++)
        ;

    aht10_init();

    uint8_t FACTORY_CALIBRATION_DATA[24];
    bmp280_init(FACTORY_CALIBRATION_DATA);

    // Flash test
    lcd1602_clear();
    lcd1602_print("Stored data, ");
    for (volatile int i = 0; i < 1000000; i++)
        ;
    float xyz = 0.0;
    w25q32_read(4, 'A', 0x50, (uint8_t *)&xyz, 4);
    uint8_t xyz_char_arr[12];
    num_float4digi_ascii(xyz, xyz_char_arr);
    lcd1602_row2();
    lcd1602_print(xyz_char_arr);
    for (volatile int i = 0; i < 1000000; i++)
        ;
    int8_t mem = 1;

    while (1)
    {
        float aht_temp = 0;
        float bmp_temp = 0;
        float humi = 0;
        float pres = 0;
        aht10_read(&aht_temp, &humi);
        bmp280_read(FACTORY_CALIBRATION_DATA, &pres, &bmp_temp);

        uint8_t aht_temp_char_arr[6];
        uint8_t bmp_temp_char_arr[6];
        uint8_t humi_char_arr[6];
        uint8_t pres_char_arr[6];
        num_float4digi_ascii(aht_temp, aht_temp_char_arr);
        num_float4digi_ascii(bmp_temp, bmp_temp_char_arr);
        num_float4digi_ascii(humi, humi_char_arr);
        num_float4digi_ascii(pres, pres_char_arr);

        lcd1602_clear();

        lcd1602_print("Temp: ");
        lcd1602_print(aht_temp_char_arr);
        lcd1602_char(0xDF);
        lcd1602_char('C');

        lcd1602_row2();
        lcd1602_print("Humi: ");
        lcd1602_print(humi_char_arr);
        lcd1602_char('%');

        for (volatile int i = 0; i < 1000000; i++)
            ;

        lcd1602_clear();
        lcd1602_print("Pres: ");
        lcd1602_print(pres_char_arr);
        lcd1602_print("hPa");

        lcd1602_row2();
        lcd1602_print("Temp: ");
        lcd1602_print(bmp_temp_char_arr);
        lcd1602_char(0xDF);
        lcd1602_char('C');
        for (volatile int i = 0; i < 1000000; i++)
            ;

        // Flash test
        if (mem == 2)
        {
            mem = 0;

            lcd1602_clear();
            lcd1602_print("Writing, ");
            for (volatile int i = 0; i < 1000000; i++)
                ;
            lcd1602_print(aht_temp_char_arr);
            for (volatile int i = 0; i < 1000000; i++)
                ;
            lcd1602_row2();
            lcd1602_print("to mem 0x50");
            for (volatile int i = 0; i < 1000000; i++)
                ;
            w25q32_write(4, 'A', 0x50, (uint8_t *)&aht_temp, 4);
            lcd1602_clear();
            lcd1602_print("Completed..");
        }
        else if (mem == 1)
            mem++;

        for (volatile int i = 0; i < 1000000; i++)
            ;
    }
}
