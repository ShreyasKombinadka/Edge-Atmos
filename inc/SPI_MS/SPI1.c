#include "SPI1.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void spi1_init(uint8_t CPOL_CHPA, uint8_t BR) // SPI1 Initialisation
{
    RCC->APB2ENR |= (1 << 12); // Enable SPI1

    GPIOB->CRL &= ~(((0xF << 16) | 0xF << 20) | (0xF << 24) | (0xF << 28)); // Clear settings of pin 4, 5, 6 & 7 (NSS, SCK1, MISO1 & MOSI1)
    GPIOB->CRL |= (0x4 << 24);                                              // pin 6(MISO1) as floating input mode
    GPIOB->CRL |= (0xB << 28) | (0xB << 20);                                // pin 5 & 7 (SCK1 & MOSI1) as 50MHz alternate push pull mode

    SPI1->CR1 = (1 << 2);  // Master mode
    SPI1->CR1 = (3 << 8);  // Ignore NSS and assume HIGH internally
    SPI1->CR1 = CPOL_CHPA; // CLK and Data read write configuration
    SPI1->CR1 = (BR << 3); // CLK devider select for SPI CLK
    SPI1->CR1 = (1 << 6);  // SPI enable
}

void spi1_slaveset(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK) // Add slave select pin
{
    switch (SLAVE_CS_PIN_BLOCK)
    {
    case 'A':                     // GPIOA
        RCC->APB2ENR |= (1 << 2); // Enable GPIOA

        if (SLAVE_CS < 16)
        {
            if (SLAVE_CS < 8)
            {
                GPIOA->CRL &= ~(0xF << SLAVE_CS * 4); // Clear configuration
                GPIOA->CRL |= (2 << SLAVE_CS * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOA->CRH &= ~(0xF << ((SLAVE_CS - 8) * 4)); // Clear configuration
                GPIOA->CRH |= (2 << ((SLAVE_CS - 8) * 4));    // Set as output at 2MHz push pull mode
            }
        }

        break;

    case 'B':                     // GPIOB
        RCC->APB2ENR |= (1 << 3); // Enable GPIOB

        if (SLAVE_CS < 16)
        {
            if (SLAVE_CS < 8)
            {
                GPIOB->CRL &= ~(0xF << SLAVE_CS * 4); // Clear configuration
                GPIOB->CRL |= (2 << SLAVE_CS * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOB->CRH &= ~(0xF << ((SLAVE_CS - 8) * 4)); // Clear configuration
                GPIOB->CRH |= (2 << ((SLAVE_CS - 8) * 4));    // Set as output at 2MHz push pull mode
            }
        }

        break;

    case 'C':                     // GPIOC
        RCC->APB2ENR |= (1 << 4); // Enable GPIOC

        if (SLAVE_CS < 16)
        {
            if (SLAVE_CS < 8)
            {
                GPIOC->CRL &= ~(0xF << SLAVE_CS * 4); // Clear configuration
                GPIOC->CRL |= (2 << SLAVE_CS * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOC->CRH &= ~(0xF << ((SLAVE_CS - 8) * 4)); // Clear configuration
                GPIOC->CRH |= (2 << ((SLAVE_CS - 8) * 4));    // Set as output at 2MHz push pull mode
            }
        }

        break;

    case 'D':                     // GPIOD
        RCC->APB2ENR |= (1 << 5); // Enable GPIOD

        if (SLAVE_CS < 16)
        {
            if (SLAVE_CS < 8)
            {
                GPIOD->CRL &= ~(0xF << SLAVE_CS * 4); // Clear configuration
                GPIOD->CRL |= (2 << SLAVE_CS * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOD->CRH &= ~(0xF << ((SLAVE_CS - 8) * 4)); // Clear configuration
                GPIOD->CRH |= (2 << ((SLAVE_CS - 8) * 4));    // Set as output at 2MHz push pull mode
            }
        }

        break;

    case 'E':                     // GPIOE
        RCC->APB2ENR |= (1 << 6); // Enable GPIOE

        if (SLAVE_CS < 16)
        {
            if (SLAVE_CS < 8)
            {
                GPIOE->CRL &= ~(0xF << SLAVE_CS * 4); // Clear configuration
                GPIOE->CRL |= (2 << SLAVE_CS * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOE->CRH &= ~(0xF << ((SLAVE_CS - 8) * 4)); // Clear configuration
                GPIOE->CRH |= (2 << ((SLAVE_CS - 8) * 4));    // Set as output at 2MHz push pull mode
            }
        }

        break;
    }
}

void spi1_slaveselect(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t SELECT) // Slave selection
{
    switch (SLAVE_CS_PIN_BLOCK)
    {
    case 'A': // GPIOA
        if (SLAVE_CS < 16)
        {
            if (SELECT)
                GPIOA->BRR = (1 << SLAVE_CS); // Reset SLAVE_CS
            else
                GPIOA->BSRR = (1 << SLAVE_CS); // Set SLAVE_CS
        }

        break;

    case 'B': // GPIOB
        if (SLAVE_CS < 16)
        {
            if (SELECT)
                GPIOB->BRR = (1 << SLAVE_CS); // Reset SLAVE_CS
            else
                GPIOB->BSRR = (1 << SLAVE_CS); // Set SLAVE_CS
        }

        break;

    case 'C': // GPIOC
        if (SLAVE_CS < 16)
        {
            if (SELECT)
                GPIOC->BRR = (1 << SLAVE_CS); // Reset SLAVE_CS
            else
                GPIOC->BSRR = (1 << SLAVE_CS); // Set SLAVE_CS
        }

        break;

    case 'D': // GPIOD
        if (SLAVE_CS < 16)
        {
            if (SELECT)
                GPIOD->BRR = (1 << SLAVE_CS); // Reset SLAVE_CS
            else
                GPIOD->BSRR = (1 << SLAVE_CS); // Set SLAVE_CS
        }

        break;

    case 'E': // GPIOE
        if (SLAVE_CS < 16)
        {
            if (SELECT)
                GPIOE->BRR = (1 << SLAVE_CS); // Reset SLAVE_CS
            else
                GPIOE->BSRR = (1 << SLAVE_CS); // Set SLAVE_CS
        }

        break;
    }
}

void spi1_8w1byte(uint8_t DATA_W) // SPI write 1byte
{
    SPI1->CR1 &= ~(1 << 11); // 8bit mode

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    (void)SPI1->DR; // Ignore the received data in DR by doing a void read to clear flags

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;
}

uint8_t spi1_8r1byte() // SPI read 1byte
{
    SPI1->CR1 &= ~(1 << 11); // 8bit mode

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = 0; // Send a dummy byte

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    uint8_t temp = SPI1->DR; // Load the received data from DR

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;

    return temp; // Return the recived data
}

void spi1_8wr1byte(uint8_t DATA_W, uint8_t DATA_R) // SPI write and read 1byte
{
    SPI1->CR1 &= ~(1 << 11); // 8bit mode

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    DATA_R = SPI1->DR; // Load the received data from DR

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;
}

void spi1_8wr1bytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t DATA_W, uint8_t DATA_R) // SPI write and read 1byte with slave selct and de-selct
{
    SPI1->CR1 &= ~(1 << 11);                           // 8bit mode
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    DATA_R = SPI1->DR; // Load the received data from DR

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave
}

void spi1_8wrnbytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t *DATA_W, uint8_t *DATA_R, int n) // SPI write and read nbyte with slave selct and de-selct
{
    SPI1->CR1 &= ~(1 << 11);                           // 8bit mode
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave

    for (volatile int i = 0; i < n; i++)
    {
        while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
            ;
        SPI1->DR = DATA_W[i]; // Load data to DR to transmit

        while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
            ;
        DATA_R[i] = SPI1->DR; // Load the received data from DR
    }

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave
}