#ifndef SPI1_H
#define SPI1_H

#include <stdint.h>

void spi1_init(uint8_t CPOL_CHPA, uint8_t BR);                                                               // SPI1 Initialisation
void spi1_slaveset(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK);                                            // Add slave select pin
void spi1_slaveselect(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t SELECT);                         // Slave selection
void spi1_8w1byte(uint8_t DATA_W);                                                                           // SPI write 1byte
uint8_t spi1_8r1byte();                                                                                      // SPI read 1byte
void spi1_8wr1byte(uint8_t DATA_W, uint8_t DATA_R);                                                          // SPI write and read 1byte
void spi1_8wr1bytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t DATA_W, uint8_t DATA_R);          // SPI write and read 1byte with slave selct and de-selct
void spi1_8wrnbytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t *DATA_W, uint8_t *DATA_R, int n); // SPI write and read nbyte with slave selct and de-selct

#endif