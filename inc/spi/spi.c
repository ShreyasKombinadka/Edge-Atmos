#include "spi.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

// SPI1 Initialisation
void spi1_init(uint8_t CPOL_CHPA, uint8_t BR)
{
    RCC->APB2ENR |= (1 << 12); // Enable SPI1

    GPIOB->CRL &= ~(((0xF << 16) | 0xF << 20) | (0xF << 24) | (0xF << 28)); // Clear settings of pin 4, 5, 6 & 7 (NSS, SCK1, MISO1 & MOSI1)
    GPIOB->CRL |= (0x4 << 24);                                              // Pin 6(MISO1) as floating input mode
    GPIOB->CRL |= (0xB << 28) | (0xB << 20);                                // Pin 5 & 7 (SCK1 & MOSI1) as 50MHz alternate push pull mode

    SPI1->CR1 = (1 << 2);  // Master mode
    SPI1->CR1 = (3 << 8);  // Ignore NSS and assume HIGH internally
    SPI1->CR1 = CPOL_CHPA; // CLK and Data read write configuration
    SPI1->CR1 = (BR << 3); // CLK devider select for SPI CLK
    SPI1->CR1 = (1 << 6);  // SPI enable
}

void spi1_nss(uint8_t PIN, uint8_t BLOCK, uint8_t SELECT) // Negative slave select
{
    switch (BLOCK)
    {
    case 'A':                     // GPIOA
        RCC->APB2ENR |= (1 << 2); // Enable GPIOA

        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOA->CRL &= ~(0xF << PIN * 4); // Clear configuration
                GPIOA->CRL |= (2 << PIN * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOA->CRH &= ~(0xF << ((PIN - 8) * 4)); // Clear configuration
                GPIOA->CRH |= (2 << ((PIN - 8) * 4));    // Set as output at 2MHz push pull mode
            }

            if (SELECT)
                GPIOA->BRR = (1 << PIN); // Reset PIN
            else
                GPIOA->BSRR = (1 << PIN); // Set PIN
        }

        break;

    case 'B':                     // GPIOB
        RCC->APB2ENR |= (1 << 3); // Enable GPIOB

        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOB->CRL &= ~(0xF << PIN * 4); // Clear configuration
                GPIOB->CRL |= (2 << PIN * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOB->CRH &= ~(0xF << ((PIN - 8) * 4)); // Clear configuration
                GPIOB->CRH |= (2 << ((PIN - 8) * 4));    // Set as output at 2MHz push pull mode
            }

            if (SELECT)
                GPIOB->BRR = (1 << PIN); // Reset PIN
            else
                GPIOB->BSRR = (1 << PIN); // Set PIN
        }

        break;

    case 'C':                     // GPIOC
        RCC->APB2ENR |= (1 << 4); // Enable GPIOC

        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOC->CRL &= ~(0xF << PIN * 4); // Clear configuration
                GPIOC->CRL |= (2 << PIN * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOC->CRH &= ~(0xF << ((PIN - 8) * 4)); // Clear configuration
                GPIOC->CRH |= (2 << ((PIN - 8) * 4));    // Set as output at 2MHz push pull mode
            }

            if (SELECT)
                GPIOC->BRR = (1 << PIN); // Reset PIN
            else
                GPIOC->BSRR = (1 << PIN); // Set PIN
        }

        break;

    case 'D':                     // GPIOD
        RCC->APB2ENR |= (1 << 5); // Enable GPIOD

        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOD->CRL &= ~(0xF << PIN * 4); // Clear configuration
                GPIOD->CRL |= (2 << PIN * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOD->CRH &= ~(0xF << ((PIN - 8) * 4)); // Clear configuration
                GPIOD->CRH |= (2 << ((PIN - 8) * 4));    // Set as output at 2MHz push pull mode
            }

            if (SELECT)
                GPIOD->BRR = (1 << PIN); // Reset PIN
            else
                GPIOD->BSRR = (1 << PIN); // Set PIN
        }

        break;

    case 'E':                     // GPIOE
        RCC->APB2ENR |= (1 << 6); // Enable GPIOE

        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOE->CRL &= ~(0xF << PIN * 4); // Clear configuration
                GPIOE->CRL |= (2 << PIN * 4);    // Set as output at 2MHz push pull mode
            }
            else
            {
                GPIOE->CRH &= ~(0xF << ((PIN - 8) * 4)); // Clear configuration
                GPIOE->CRH |= (2 << ((PIN - 8) * 4));    // Set as output at 2MHz push pull mode
            }

            if (SELECT)
                GPIOE->BRR = (1 << PIN); // Reset PIN
            else
                GPIOE->BSRR = (1 << PIN); // Set PIN
        }

        break;
    }
}

void spi1_8wr1byte(uint8_t *DATA)
{
    SPI1->CR1 &= ~(1 << 11); // 8bit mode
    spi1_nss(4, 'A', 1);     // Select slave

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = *DATA;       // Load data to be send
    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;

    *DATA = SPI1->DR; // Load the received data

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;

    spi1_nss(4, 'A', 0); // De-select slave
}