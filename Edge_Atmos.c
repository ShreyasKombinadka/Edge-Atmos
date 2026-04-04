#include "stm32f10xx.h"

int main(void)
{

    uint8_t addr = 0xAA; // Yes i know this is wrong i donnt know the actual adresss os oi wrote it for noq i wiill chage oot later for now ignore it
    uint8_t reg = 0xAA;  // Yes i know this is wrong i donnt know the actual adresss os oi wrote it for noq i wiill chage oot later for now ignore it
    uint8_t temp_msb = 0;
    uint8_t temp_lsb = 0;
    uint16_t temp = 0;

    RCC->APB2ENR |= (1 << 3);
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

        I2C1->CR1 |= (1 << 8);
        while (!(I2C1->SR1 & 1))
            ;

        I2C1->DR = (addr << 1) | 0;
        while (!(I2C1->SR1 & (1 << 1)))
            ;
        (void)I2C1->SR2;

        I2C1->DR = reg;
        while (!(I2C1->SR1 & (1 << 7)))
            ;
        while (!(I2C1->SR1 & (1 << 2)))
            ;

        I2C1->CR1 |= (1 << 8);
        while (!(I2C1->SR1 & 1))
            ;

        I2C1->DR = (addr << 1) | 1;
        while (!(I2C1->SR1 & (1 << 1)))
            ;
        (void)I2C1->SR2;

        I2C1->CR1 &= ~(1 << 10);
        while (!(I2C1->SR1 & (1 << 6)))
            ;
        temp_msb = I2C1->DR;
        while (!(I2C1->SR1 & (1 << 6)))
            ;
        temp_lsb = I2C1->DR;
        I2C1->CR1 |= (1 << 9);

        temp = (temp_msb << 8) | temp_lsb;
    }
}
