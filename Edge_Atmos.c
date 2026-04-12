#include <stdint.h>
#include "i2c.h"
#include "ascii.h"

#define STM32F103xB
#include "stm32f1xx.h"

int main(void)
{

    uint8_t temp_addr = 0xAA;
    uint8_t temp_reg = 0xAA;
    uint8_t lcd_addr = 0x27;

    uint16_t temp = 0;
    uint16_t disp_temp = 0;

    struct I2C i2c;
    i2c.FREQ = 8;
    i2c.CCR = 40;
    i2c.TRISE = 9;
    I2C1_start();

    while (1)
    {
        I2C1_wws1bint(temp_addr, temp_reg);

        temp = I2C1_rs2bint(temp_addr);

        // printing the values on the LCD display
        disp_temp = temp;

        I2C1_wake(lcd_addr);

        uint8_t i = 0;
        while (disp_temp > 0)
        {
            char num = num_to_ascii((disp_temp / (-9 * (i) + 10)) % (-90 * (i) + 100));
            i++;

            I2C1_w1bint(num);
        }
        I2C1_stop();
    }
}