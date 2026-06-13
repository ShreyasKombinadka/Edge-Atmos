#include <stdint.h>
#include "./i2c/i2c.h"
#include "./ascii/ascii.h"
#include "./lcd/lcd.h"

#define STM32F103xB
#include "stm32f1xx.h"

int main(void)
{
    uint8_t lcd_addr = 0x27;

    i2c1_init();
    lcd_init(lcd_addr);

    while (1)
    {
        uint8_t *arr;
        uint8_t i = 0;
        while (i >= 0 && i < 100)
        {
            arr = num_to_ascii(i);

            lcd_wwsc(lcd_addr, 1);
            for (volatile int x = 0; x < 5; x++)
                lcd_wws1byte(lcd_addr, arr[x]);

            i++;
            for (volatile int j = 0; j < 10000; j++)
                ;
        }
    }
}
