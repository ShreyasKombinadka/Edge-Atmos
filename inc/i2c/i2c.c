#include "i2c.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

// Start I2C1 block
void i2c1_init(void)
{
    RCC->APB2ENR |= (1 << 3);
    RCC->APB1ENR |= (1 << 21);

    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB->CRL |= (0xE << 24);
    GPIOB->CRL |= (0xE << 28);

    I2C1->CR2 = 8;
    I2C1->CCR = 40;
    I2C1->TRISE = 9;
    I2C1->CR1 |= 1;
}

// Wake the target device
void i2c1_wake(uint8_t addr)
{
    I2C1->CR1 |= (1 << 8);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 0;
    while (!(I2C1->SR1 & (1 << 1)))
        ;
    (void)I2C1->SR2;
}

// Write 1byte of data(int)
void i2c1_w1byte(uint8_t data)
{
    I2C1->DR = data;
    while (!(I2C1->SR1 & (1 << 7)))
        ;
    while (!(I2C1->SR1 & (1 << 2)))
        ;
}

// Stop the I2C1
void i2c1_stop(void)
{
    I2C1->CR1 |= (1 << 9);
}

// Wake the target & Write the 1byte of data(int) & Stop the I2C1
void i2c1_wws1byte(uint8_t addr, uint8_t data)
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

    i2c1_stop();
}

// Read the 1byte of data(int) & Stop the I2C1
uint8_t i2c1_rs1byte(uint8_t addr)
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

    i2c1_stop();

    return I2C1->DR;
}

// Read the 2byte of data(int) & Stop the I2C1
uint16_t i2c1_rs2byte(uint8_t addr)
{
    uint16_t data = 0;

    I2C1->CR1 |= (1 << 8);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 1;
    while (!(I2C1->SR1 & (1 << 1)))
        ;
    (void)I2C1->SR2;

    while (!(I2C1->SR1 & (1 << 6)))
        ;
    I2C1->CR1 &= ~(1 << 10);
    data = I2C1->DR;
    while (!(I2C1->SR1 & (1 << 6)))
        ;
    data = (data << 8) | I2C1->DR;

    i2c1_stop();

    return data;
}

void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n)
{
    I2C1->CR1 |= (1 << 8);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 1;
    while (!(I2C1->SR1 & (1 << 1)))
        ;
    (void)I2C1->SR2;

    int i = 0;
    while (i <= (n - 1) && n != 0)
    {
        if (i >= n - 2)
            I2C1->CR1 &= ~(1 << 10);

        while (!(I2C1->SR1 & (1 << 6)))
            ;

        data_addr[i] = I2C1->DR;

        i++;
    }

    i2c1_stop();
}