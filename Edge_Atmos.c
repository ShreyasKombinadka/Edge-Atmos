#include <stdint.h>
#include "./i2c/i2c.h"
#include "./ascii/ascii.h"
#include "./lcd/lcd.h"

#define STM32F103xB
#include "stm32f1xx.h"

int main(void)
{
    uint8_t lcd_addr = 0x27;

    I2C1_init();

    while (1)
    {
        I2C1_wake(lcd_addr);

        uint8_t *num;
        uint8_t i = 0;
        while (i >= 0 && i < 100)
        {
            num = num_to_ascii(i);
            i++;

            lcd_w1byte(lcd_addr, num[4]);

            for (volatile int j = 0; j < 10000; j++)
                ;
        }

        I2C1_stop();
    }
}
