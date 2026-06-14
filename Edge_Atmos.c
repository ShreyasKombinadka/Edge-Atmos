#include <stdint.h>
#include "./i2c/i2c.h"
#include "./ascii/ascii.h"
#include "./lcd/lcd.h"
#include "./AHT10/AHT10.h"

#define STM32F103xB
#include "stm32f1xx.h"

int main(void)
{
    uint8_t lcd_addr = 0x27;

    i2c1_init();
    lcd_debug(lcd_addr, "i2c done..!");

    lcd_init(lcd_addr);
    lcd_wwscmd(lcd_addr, 1);
    lcd_debug(lcd_addr, "lcd done..!");

    lcd_debug(lcd_addr, "aht10 start..!");
    aht10_init();
    lcd_debug(lcd_addr, "aht10 done..!");

    while (1)
    {
        uint8_t *arr;
        uint8_t i = 0;
        uint32_t temp = aht10_read();

        arr = num_to_ascii(temp << 4);

        lcd_wwscmd(lcd_addr, 1);
        lcd_wwschar(lcd_addr, "Temp: ");

        for (volatile int x = 0; x < 5; x++)
            lcd_wws1byte(lcd_addr, arr[x]);

        i++;
        for (volatile int j = 0; j < 1000000; j++)
            ;
    }
}
