#include "SPI1.h"
#include "./GPIO_MS/GPIO.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

/* SPI1 Initialisation
CPHA & CPOL
Bit 0: Clock phase(CPHA).
Bit 1: Clock polarity(CPOL).
Allowed values:
0: FirstEdge: The first clock transition is the first data capture edge & IdleLow: CK to 0 when idle
1: SecondEdge: The second clock transition is the first data capture edge & IdleLow: CK to 0 when idle
2: FirstEdge: The first clock transition is the first data capture edge & IdleHigh: CK to 1 when idle
3: SecondEdge: The second clock transition is the first data capture edge & IdleHigh: CK to 1 when idle

BR
3 bits : Baud rate control.
Allowed values:
0: Div2: f_PCLK / 2
1: Div4: f_PCLK / 4
2: Div8: f_PCLK / 8
3: Div16: f_PCLK / 16
4: Div32: f_PCLK / 32
5: Div64: f_PCLK / 64
6: Div128: f_PCLK / 128
7: Div256: f_PCLK / 256
*/
void spi1_init(uint8_t CPOL_CPHA, uint8_t BR) // SPI1 Initialisation00
{
    RCC->APB2ENR |= (1 << 12); // Enable SPI1
    gpio_en('A');              // Enable GPIOA PORT

    gpio_setup(5, 'A', 3, 2); // SCK1 (50MHz alternate push pull)
    gpio_setup(6, 'A', 0, 1); // MISO1 (Floating input)
    gpio_setup(7, 'A', 3, 2); // MOSI1 (50MHz alternate push pull)

    SPI1->CR1 |= (1 << 2);           // Master mode
    SPI1->CR1 |= (3 << 8);           // Ignore NSS and assume HIGH internally
    SPI1->CR1 &= ~3;                 // CLK and Data read write bits clear
    SPI1->CR1 |= (CPOL_CPHA & 0x03); // CLK and Data read write configuration
    SPI1->CR1 &= ~(7 << 3);          // CLK devider select bits clear
    SPI1->CR1 |= ((BR & 0x07) << 3); // CLK devider select for SPI CLK
    SPI1->CR1 &= ~(1 << 11);         // 8bit mode
    SPI1->CR1 |= (1 << 6);           // SPI enabled
}

void spi1_slaveset(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t MODE) // Add slave select pin
{
    gpio_en(SLAVE_CS_PIN_PORT); // Enable GPIO PORT

    gpio_setup(SLAVE_CS, SLAVE_CS_PIN_PORT, MODE, 0); // Setup push pull mode

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device by default
}

void spi1_slaveselect(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t SELECT) // Slave selection
{
    if (SELECT)
        gpio_setreset(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // Pull the slave line low
    else
        gpio_setreset(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Pull the slave line high
}

void spi1_8wf1byte(uint8_t DATA_W) // SPI fast write 1byte
{
    while (!(SPI1->SR & (1 << 1))) // Wait till the Tx buffer is empty
        ;
    SPI1->DR = DATA_W; // Load data to DR to transmit
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