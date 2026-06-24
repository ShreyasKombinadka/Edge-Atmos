#ifndef SPI1_H
#define SPI1_H

#include <stdint.h>

void spi1_init(uint8_t CPOL_CHPA, uint8_t BR);                        // SPI1 Initialisation
void spi1_slaveset(uint8_t PIN, uint8_t BLOCK);                       // Add slave select pin
void spi1_slaveselect(uint8_t PIN, uint8_t BLOCK, uint8_t SELECT);    // Slave selection
void spi1_8wr1byte(uint8_t PIN, uint8_t BLOCK, uint8_t DATA);         // SPI write and read 1byte
void spi1_8wrnbyte(uint8_t PIN, uint8_t BLOCK, uint8_t *DATA, int n); // SPI write and read nbyte

#endif