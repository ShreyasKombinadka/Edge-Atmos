#ifndef SPI1_H
#define SPI1_H

#include <stdint.h>

void spi1_init(uint8_t CPOL_CPHA, uint8_t BR);
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
Bits 3-5: Baud rate control.

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

void spi1_slaveset(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK);                                            // Add slave select pin
void spi1_slaveselect(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t SELECT);                         // Slave selection
void spi1_8w1byte(uint8_t DATA_W);                                                                           // SPI write 1byte
uint8_t spi1_8r1byte();                                                                                      // SPI read 1byte
uint8_t spi1_8wr1byte(uint8_t DATA_W);                                                                       // SPI write and read 1byte
uint8_t spi1_8wr1bytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t DATA_W);                       // SPI write and read 1byte with slave selct and de-selct
void spi1_8wrnbytesd(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t *DATA_W, uint8_t *DATA_R, int n); // SPI write and read nbyte with slave selct and de-selct

#endif