#include "i2c.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

struct I2C i2c;

void I2C1_start(void)
{
    RCC->APB2ENR |= (1 << 3);
    RCC->APB1ENR |= (1 << 21);

    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB->CRL |= (0xE << 24);
    GPIOB->CRL |= (0xE << 28);

    I2C1->CR2 = i2c.FREQ;
    I2C1->CCR = i2c.CCR;
    I2C1->TRISE = i2c.TRISE;
    I2C1->CR1 |= 1;
}

void I2C1_write(uint8_t addr, uint8_t data)
{
    I2C1->CR1 |= (1 << 8);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 0;
    while (!(I2C1->SR1 & (1 << 1)))
        ;
    (void)I2C1->SR2;

    I2C1->DR = data;
    while (!(I2C1->SR1 & (1 << 7)))
        ;
    while (!(I2C1->SR1 & (1 << 2)))
        ;
}

uint8_t I2C1_read1b(uint8_t addr)
{
    I2C1->CR1 |= (1 << 8);
    I2C1->CR1 &= ~(1 << 10);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 1;
    while (!(I2C1->SR1 & (1 << 1)))
        ;
    (void)I2C1->SR2;

    while (!(I2C1->SR1 & (1 << 6)))
        ;

    I2C1->CR1 |= (1 << 9);

    return I2C1->DR;
}

uint16_t I2C1_read2b(uint8_t addr)
{
    uint16_t data = 0;

    I2C1->CR1 |= (1 << 8);
    I2C1->CR1 &= ~(1 << 10);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 1;
    while (!(I2C1->SR1 & (1 << 1)))
        ;
    (void)I2C1->SR2;

    while (!(I2C1->SR1 & (1 << 6)))
        ;

    data = I2C1->DR;
    while (!(I2C1->SR1 & (1 << 6)))
        ;
    data = (data << 8) | I2C1->DR;

    I2C1->CR1 |= (1 << 9);

    return data;
}

void I2C1_stop(void)
{
    I2C1->CR1 |= (1 << 9);
}