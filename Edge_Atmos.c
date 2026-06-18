#include <stdint.h>
#include "./i2c/i2c.h"
#include "./ascii/ascii.h"
#include "./lcd/lcd.h"
#include "./AHT10/AHT10.h"
#include "./BMP280/BMP280.h"

#define STM32F103xB
#include "./STM32F103_CMSIS/stm32f1xx.h"

int main(void)
{
    uint8_t lcd_addr = 0x27;

    i2c1_init();
    lcd_init(lcd_addr);
    lcd_print(lcd_addr, "Edge Atmos");
    for (volatile int i = 0; i < 1000000; i++)
        ;

    aht10_init();

    uint16_t FACTORY_CALIBRATION_DATA[24];
    bmp280_init(FACTORY_CALIBRATION_DATA);

    while (1)
    {
        float aht_temp = 0.0;
        float bmp_temp = 0.0;
        float humi = 0.0;
        uint32_t pres = 0;
        float *aht_temp_ptr = &aht_temp;
        float *bmp_temp_ptr = &bmp_temp;
        float *humi_ptr = &humi;
        aht10_read(aht_temp_ptr, humi_ptr);
        bmp280_read(FACTORY_CALIBRATION_DATA, pres, bmp_temp_ptr);

        uint8_t aht_temp_char_arr[6];
        uint8_t bmp_temp_char_arr[6];
        uint8_t humi_char_arr[6];
        uint8_t pres_char_arr[6];
        num_00_00_ascii(*aht_temp_ptr, aht_temp_char_arr);
        num_00_00_ascii(*bmp_temp_ptr, bmp_temp_char_arr);
        num_00_00_ascii(*humi_ptr, humi_char_arr);
        num_00_00_ascii(pres, pres_char_arr);

        lcd_clear(lcd_addr);

        lcd_print(lcd_addr, "Temp: ");
        lcd_print(lcd_addr, aht_temp_char_arr);
        lcd_char(lcd_addr, 0xDF);
        lcd_char(lcd_addr, 'C');

        lcd_row2(lcd_addr);
        lcd_print(lcd_addr, "Humi: ");
        lcd_print(lcd_addr, humi_char_arr);
        lcd_char(lcd_addr, '%');

        for (volatile int i = 0; i < 1000000; i++)
            ;

        lcd_clear(lcd_addr);
        lcd_print(lcd_addr, "Pres: ");
        lcd_print(lcd_addr, pres_char_arr);
        lcd_string(lcd_addr, 'psi');

        lcd_row2(lcd_addr);
        lcd_print(lcd_addr, "Temp: ");
        lcd_print(lcd_addr, bmp_temp_char_arr);
        lcd_char(lcd_addr, 0xDF);
        lcd_char(lcd_addr, 'C');

        for (volatile int i = 0; i < 1000000; i++)
            ;
    }
}
