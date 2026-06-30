#include "SPI1.h"
#include "./GPIO_MS/GPIO.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void spi1_init(uint8_t CPOL_CPHA, uint8_t BR) // SPI1 Initialisation00
{
    RCC->APB2ENR |= (1 << 12); // Enable SPI1
    gpio_en('A');              // Enable GPIOA PORT

    gpio_setup(5, 'A', 11); // SCK1 (50MHz alternate push pull)
    gpio_setup(6, 'A', 4);  // MISO1 (Floating input)
    gpio_setup(7, 'A', 11); // MOSI1 (50MHz alternate push pull)

    SPI1->CR1 |= (1 << 2);           // Master mode
    SPI1->CR1 |= (3 << 8);           // Ignore NSS and assume HIGH internally
    SPI1->CR1 &= ~3;                 // CLK and Data read write bits clear
    SPI1->CR1 |= (CPOL_CPHA & 0x03); // CLK and Data read write configuration
    SPI1->CR1 &= ~(7 << 3);          // CLK devider select bits clear
    SPI1->CR1 |= ((BR & 0x07) << 3); // CLK devider select for SPI CLK
    SPI1->CR1 &= ~(1 << 11);         // 8bit mode
    SPI1->CR1 |= (1 << 6);           // SPI enabled
}

void spi1_slaveset(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT) // Add slave select pin
{
    gpio_en(SLAVE_CS_PIN_PORT); // Enable GPIO PORT

    gpio_setup(SLAVE_CS, SLAVE_CS_PIN_PORT, 2); // Set as output at 2MHz push pull mode
}

void spi1_slaveselect(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t SELECT) // Slave selection
{
    if (SELECT)
        gpio_setreset(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // Pull the slave line low
    else
        gpio_setreset(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Pull the slave line high
}

void spi1_8w1byte(uint8_t DATA_W) // SPI write 1byte
{
    (void)SPI1->SR; // Clear flags
    (void)SPI1->DR; // Clear DR

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    (void)SPI1->DR; // Clear DR

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;
}

uint8_t spi1_8r1byte() // SPI read 1byte
{
    (void)SPI1->SR; // Clear flags
    (void)SPI1->DR; // Clear DR

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

uint8_t spi1_8wr1byte(uint8_t DATA_W) // SPI write and read 1byte
{
    (void)SPI1->SR; // Clear flags
    (void)SPI1->DR; // Clear DR

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    uint8_t DATA_R = SPI1->DR; // Load the received data from DR

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;

    return DATA_R;
}

uint8_t spi1_8wr1bytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t DATA_W) // SPI write and read 1byte with slave selct and de-selct
{
    (void)SPI1->SR; // Clear flags
    (void)SPI1->DR; // Clear DR

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave

    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit

    while (!(SPI1->SR & 1)) // Wait till the Rx buffer is not empty
        ;
    uint8_t DATA_R = SPI1->DR; // Load the received data from DR

    while (SPI1->SR & (1 << 7)) // Wait till SPI is completed and DR is free
        ;

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave

    return DATA_R;
}

void spi1_8wrnbytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t *DATA_W, uint8_t *DATA_R, int n) // SPI write and read nbyte with slave selct and de-selct
{
    (void)SPI1->SR; // Clear flags
    (void)SPI1->DR; // Clear DR

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave

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

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave
}