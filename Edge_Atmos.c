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
    bmp280_init();

    while (1)
    {
        float temp = 0.0;
        float humi = 0.0;
        float *temp_ptr = &temp;
        float *humi_ptr = &humi;

        uint8_t temp_char_arr[6];
        uint8_t humi_char_arr[6];

        aht10_read(temp_ptr, humi_ptr);

        num_00_00_ascii(*temp_ptr, temp_char_arr);
        num_00_00_ascii(*humi_ptr, humi_char_arr);

        lcd_clear(lcd_addr);

        lcd_print(lcd_addr, "Temp: ");
        lcd_print(lcd_addr, temp_char_arr);
        lcd_char(lcd_addr, 0xDF);
        lcd_char(lcd_addr, 'C');

        lcd_row2(lcd_addr);
        lcd_print(lcd_addr, "Humi: ");
        lcd_print(lcd_addr, humi_char_arr);
        lcd_char(lcd_addr, '%');

        for (volatile int i = 0; i < 1000000; i++)
            ;
    }
}
