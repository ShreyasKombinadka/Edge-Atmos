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

    uint8_t temp_msb = 0;
    uint8_t temp_lsb = 0;
    uint16_t temp = 0;
    uint16_t disp_temp = 0;

    RCC->APB1ENR |= (1 << 21);

    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB->CRL |= (0xE << 24);
    GPIOB->CRL |= (0xE << 28);

    I2C1->CR2 = 8;
    I2C1->CCR = 40;
    I2C1->TRISE = 9;
    I2C1->CR1 |= 1;

    while (1)
    {

        struct I2C i2c;

        // printing the values on the LCD display
        disp_temp = temp;

        I2C1->CR1 |= (1 << 8);
        while (!(I2C1->SR1 & 1))
            ;
        I2C1->DR = (lcd_addr << 1) | 0;
        while (!(I2C1->SR1 & (1 << 1)))
            ;
        (void)I2C1->SR2;

        uint8_t i = 0;
        while (disp_temp > 0)
        {
            char num = num_to_ascii((disp_temp / (-9 * (i) + 10)) % (-90 * (i) + 100));
            I2C1->DR = num;
            while (!(I2C1->SR1 & (1 << 7)))
                ;
            while (!(I2C1->SR1 & (1 << 2)))
                ;
            i++;
        }
        I2C1->CR1 |= (1 << 9);
    }
}