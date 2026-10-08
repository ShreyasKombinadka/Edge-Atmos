#include "I2C1.h"
#include "../GPIO_MS/GPIO.h"
#include <stdint.h>

#define STM32F103xB
#include "../STM32F103_CMSIS/stm32f1xx.h"

static I2C i2c1;

void i2c1_init(void) // Start I2C1 block
{
    RCC->APB1ENR |= (1U << 21); // Enable I2C1 block
    gpio_en('B');               // Enable GPIOB port

    gpio_setup(6U, 'B', 2, 3); // SCL1(Alternate open drain)
    gpio_setup(7U, 'B', 2, 3); // SDA1(Alternate open drain)

    I2C1->CR2 = 8;
    I2C1->CCR = 40;
    I2C1->TRISE = 9;
    I2C1->CR1 |= 1;

    I2C1->CR2 |= (1U << 9);  // Event interrupt enable
    I2C1->CR2 |= (1U << 10); // Buffer interrupt enable

    NVIC->ISER[0] = (1U << 31); // I2C1 event interrupt
}

void i2c1_wake(uint8_t ADDR) // Wake the target device
{
    i2c1.STATE = START;     // I2C seqence started
    I2C1->CR1 |= (1U << 8); // Send start bit

    i2c1.ADDR = ADDR; // Save adress for interupt functions
}

void I2C1_IRQHandler(void) // I2C1 interupt handler
{
    if (i2c1.STATE == START) // Start bit sent
    {
        i2c1.STATE = SB;                 // Start bit sent state
        I2C1->DR = (i2c1.ADDR << 1) | 0; // Send address
    }
    else if (i2c1.STATE == SB) // Address sent
    {
        i2c1.STATE = ADDR; // Address sent state
        (void)I2C1->SR2;   // Clear status registers

        // Byte counter and flag reset
        i2c1.W_COUNT = 0; // Byte counter cleared
        i2c1.N_BYTE = 0;  // N byte flag cleared
    }
    else if (i2c1.STATE == ADDR || i2c1.Tx == 1) // Address sent or Tx data ready
    {
        i2c1.STATE = TxE;     // Tx buffer empty stat
        if (i2c1.N_BYTE == 1) // N byte flag not set
        {
            I2C1->DR = i2c1.W_DATA[i2c1.W_COUNT]; // Data too be sent

            if (i2c1.W_COUNT >= (i2c1.W_LENGTH - 1)) // N byte
                i2c1.N_BYTE = 1;                     // N byte flag set
            else                                     // Not N byte
                i2c1.N_BYTE = 0;                     // N byte flag cleared

            i2c1.W_COUNT++; // Incriment byte counter
        }
    }
    else if (i2c1.STATE == TxE) // Tx buffer empty
    {
        i2c1.STATE = BTF;                         // Byte transfer finsihs state
        if (i2c1.N_BYTE == 0)                     // LN byte flag set
            I2C1->DR = i2c1.W_DATA[i2c1.W_COUNT]; // Data too be sent
    }
}

// Write 1byte of data(int)
void i2c1_w1byte(uint8_t DATA)
{
    if (i2c1.STATE == ADDR || i2c1.STATE == BTF) // Address or byte sent
    {
        i2c1.Tx == 1;                     // Tx data ready flag
        i2c1.W_COUNT = 0;                 // Byte counter cleared
        i2c1.W_DATA[i2c1.W_COUNT] = DATA; // Save the data to be sent
        i2c1.N_BYTE = 0;                  // 1 byte
    }
}

// Write nbyte of data(int)
void i2c1_wnbyte(uint8_t *DATA, uint8_t LENGTH)
{
    if (i2c1.STATE == ADDR || i2c1.STATE == BTF) // Address or byte sent
    {
        i2c1.Tx == 1;        // Tx data ready flag
        i2c1.W_COUNT = 0;    // Byte counter cleared
        i2c1.W_DATA = *DATA; // Save the data to be sent
        i2c1.N_BYTE = 1;     // N byte
    }
}

// Stop the I2C1
void i2c1_stop(void)
{
    i2c1.STATE = STOP;
    I2C1->CR1 |= (1U << 9);
}

// Read the n bytes of data & Stop the I2C1
void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n)
{
    I2C1->CR1 |= (1U << 10);

    I2C1->CR1 |= (1U << 8);
    while (!(I2C1->SR1 & 1))
        ;

    I2C1->DR = (addr << 1) | 1;
    while (!(I2C1->SR1 & (1U << 1)))
        ;
    (void)I2C1->SR2;

    int i = 0;
    while (i <= (n - 1) && n != 0)
    {
        if (i >= n - 2)
            I2C1->CR1 &= ~(1 << 10);

        while (!(I2C1->SR1 & (1U << 6)))
            ;

        data_addr[i] = I2C1->DR;

        i++;
    }

    i2c1_stop();
}