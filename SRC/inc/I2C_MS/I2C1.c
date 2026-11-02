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

        buff_rst(3); // Buffer clear
    }
    else if (i2c1.STATE == ADDR || i2c1.N_BYTE == 1) // Address sent or N byte flag set
    {
        i2c1.STATE = TxE; // Tx buffer empty stat
        if (buff_read(2)) // N byte flag set and counter is less than total byte count
        {
            I2C1->DR = i2c1.W_DATA[i2c1.W_COUNT]; // Data too be sent
            i2c1.W_COUNT++;                       // Incriment byte counter
        }
        else
            i2c1.N_BYTE == 0; // N byte flag reset
    }
    else if (i2c1.STATE == TxE) // Tx buffer empty
    {
        i2c1.STATE = BTF; // Byte transfer finsihs state
    }
}

// Write 1byte of data(int)
void i2c1_w1byte(uint8_t DATA)
{
    buff_write(DATA); // Write data to buffer
}

// Write nbyte of data(int)
void i2c1_wnbyte(uint8_t *DATA, uint8_t LENGTH)
{
    for (volatile int i = 0; i < LENGTH; i++)
    {
        buff_write(DATA[i]); // Write data to buffer
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

// Helper functions
void buff_rst(uint8_t BUFF_SELECT) // Buffer reset
{
    switch (BUFF_SELECT)
    {
    case 0:                                          // Addr buffer
        i2c1.W_ABUFF_ADDR = 0;                       // Addr buff write addr clear
        i2c1.R_ABUFF_ADDR = 0;                       // Addr buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Addr buff clear
            i2c1.ADDR_BUFF[i] = 0;
        break;
    case 1:                                          // Data buffer
        i2c1.W_DBUFF_ADDR = 0;                       // Data buff write addr clear
        i2c1.R_DBUFF_ADDR = 0;                       // Data buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Data buff clear
            i2c1.DATA_BUFF[i] = 0;
        break;
    case 2:                                          // End buffer
        i2c1.W_EBUFF_ADDR = 0;                       // End buff write addr clear
        i2c1.R_EBUFF_ADDR = 0;                       // End buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // End buff clear
            i2c1.END_BUFF[i] = 0;
        break;
    case 3: // All buffer reset
        // Addr buffer
        i2c1.W_ABUFF_ADDR = 0;                       // Addr buff write addr clear
        i2c1.R_ABUFF_ADDR = 0;                       // Addr buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Addr buff clear
            i2c1.ADDR_BUFF[i] = 0;

        // Data buffer
        i2c1.W_DBUFF_ADDR = 0;                       // Data buff write addr clear
        i2c1.R_DBUFF_ADDR = 0;                       // Data buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Data buff clear
            i2c1.DATA_BUFF[i] = 0;

        // End buffer
        i2c1.W_EBUFF_ADDR = 0;                       // End buff write addr clear
        i2c1.R_EBUFF_ADDR = 0;                       // End buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // End buff clear
            i2c1.END_BUFF[i] = 0;
        break;
    default:
        break;
    }
}

void buff_write(uint8_t BUFF_SELECT, uint8_t DATA) // Buffer write
{
    switch (BUFF_SELECT)
    {
    case 0: // Addr buffer
        if (i2c1.W_ABUFF_ADDR > BUFF_SIZE - 1)
            i2c1.W_ABUFF_ADDR = 0; // Buff write addr reset
        i2c1.ADDR_BUFF[i2c1.W_ABUFF_ADDR] = DATA;
        i2c1.W_ABUFF_ADDR++; // Incrimnet buff location
        break;
    case 1: // Data buffer
        if (i2c1.W_DBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.W_DBUFF_ADDR = 0; // Buff write addr reset
        i2c1.DATA_BUFF[i2c1.W_DBUFF_ADDR] = DATA;
        i2c1.W_DBUFF_ADDR++; // Incrimnet buff location
        break;
    case 2: // End buffer
        if (i2c1.W_EBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.W_EBUFF_ADDR = 0; // Buff write addr reset
        i2c1.END_BUFF[i2c1.W_EBUFF_ADDR] = DATA;
        i2c1.W_EBUFF_ADDR++; // Incrimnet buff location
        break;
    default:
        break;
    }
}

uint8_t buff_read(uint8_t BUFF_SELECT) // Buffer read
{
    uint8_t data_temp = 0; // Temp data varibale
    switch (BUFF_SELECT)
    {
    case 0: // Addr buffer
        if (i2c1.R_ABUFF_ADDR > BUFF_SIZE - 1)
            i2c1.R_ABUFF_ADDR = 0; // Buff read addr reset
        data_temp = i2c1.ADDR_BUFF[i2c1.R_ABUFF_ADDR];
        i2c1.R_ABUFF_ADDR++; // Incrimnet buff location
        break;
    case 1: // Data buffer
        if (i2c1.R_DBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.R_DBUFF_ADDR = 0; // Buff read addr reset
        data_temp = i2c1.DATA_BUFF[i2c1.R_DBUFF_ADDR];
        i2c1.R_DBUFF_ADDR++; // Incrimnet buff location
        break;
    case 2: // End buffer
        if (i2c1.R_EBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.R_EBUFF_ADDR = 0; // Buff read addr reset
        data_temp = i2c1.END_BUFF[i2c1.R_EBUFF_ADDR];
        i2c1.R_EBUFF_ADDR++; // Incrimnet buff location
        break;
    default:
        break;
    }

    return data_temp; // Return data
}