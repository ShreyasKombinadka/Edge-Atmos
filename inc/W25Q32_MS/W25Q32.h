#ifndef W25Q32_H
#define W25Q32_H

#include <stdint.h>

void w25q32_read(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t *MEM_LOCATION_24);

#endif