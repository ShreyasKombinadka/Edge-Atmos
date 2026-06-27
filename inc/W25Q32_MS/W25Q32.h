#ifndef W25Q32_H
#define W25Q32_H

#include <stdint.h>

void w25q32_read(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint32_t MEM_LOCATION_24, uint8_t *DATA, uint8_t DATA_SIZE_BYTE);  // Flash memory read
void w25q32_write(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint32_t MEM_LOCATION_24, uint8_t *DATA, uint8_t DATA_SIZE_BYTE); // Flash memory write

#endif