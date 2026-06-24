#include "W25Q32.h"
#include "../SPI_MS/SPI1.h"
#include <stdint.h>

void w25q32_read(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint8_t *MEM_LOCATION_24, uint8_t DATA) // Flash memory read
{
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave device

    spi1_8w1byte(0x03); // Write memory read cmd byte
    for (volatile int i = 0; i < 3; i++)
    {
        spi1_8w1byte(MEM_LOCATION_24[i]); // Send 24bit memroy adress with MSB first
    }
    DATA = spi1_8r1byte(); // Data at the mem location

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave device
}