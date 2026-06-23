#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi1_init(uint8_t CPOL_CHPA, uint8_t BR); // SPI1 initialisation
void spi1_nss(uint8_t PIN, uint8_t BLOCK);     // Negative slave select, call before write/read function to select the slave device
void spi1_wr1byte(uint8_t *DATA);

#endif