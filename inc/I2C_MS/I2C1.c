#include "I2C1.h"
#include "./GPIO_MS/GPIO.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

// Start I2C1 block
void i2c1_init(void)
{
    RCC->APB1ENR |= (1 << 21); // Enable I2C1 block
    gpio_en('B');              // Enable GPIOB port

    gpio_setup(6, 'B', 14); // SCL1(Alternate open drain)
    gpio_setup(7, 'B', 14); // SDA1(Alternate open drain)

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

// Read the n bytes of data & Stop the I2C1
void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n)
{
    I2C1->CR1 |= (1 << 10);

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