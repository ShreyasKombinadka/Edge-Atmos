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
    lcd_init(lcd_addr);
    lcd_clear(lcd_addr);
    lcd_debug(lcd_addr, "lcd done..!");

    lcd_debug(lcd_addr, "aht10 start..!");
    aht10_init();
    lcd_debug(lcd_addr, "aht10 done..!");

    while (1)
    {
        uint8_t *arr;

        uint8_t *temp;
        uint8_t *humi;
        aht10_read(temp, humi);

        arr = num_to_ascii(*temp);

        lcd_wwscmd(lcd_addr, 1);
        lcd_wwschar(lcd_addr, "Temp: ");

        for (volatile int x = 0; x < 5; x++)
            lcd_wws1byte(lcd_addr, arr[x]);

        for (volatile int j = 0; j < 1000000; j++)
            ;
    }
}
