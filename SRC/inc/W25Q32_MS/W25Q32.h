#ifndef W25Q32_H
#define W25Q32_H

#include <stdint.h>

struct W25Q32
{
    uint8_t SLAVE_CS;
    uint8_t SLAVE_CS_PIN_PORT;
};

void w25q32_init(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT);                      // Flash initialisation
void w25q32_read(uint32_t MEM_LOCATION_24, uint8_t *DATA, uint8_t DATA_SIZE_BYTE);  // Flash memory read
void w25q32_write(uint32_t MEM_LOCATION_24, uint8_t *DATA, uint8_t DATA_SIZE_BYTE); // Flash memory write
void w25q32_sectorclear(uint32_t MEM_LOCATION_24);                                  // Sector clear

#endif