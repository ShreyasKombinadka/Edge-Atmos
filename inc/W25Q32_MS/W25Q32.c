#include "W25Q32.h"
#include "../SPI_MS/SPI1.h"
#include <stdint.h>

int8_t w25q32_r1byte(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint32_t MEM_LOCATION_24) // Flash memory read 1byte
{
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave device

    spi1_8w1byte(0x03); // Read enable cmd byte
    for (volatile int i = 0; i < 3; i++)
    {
        spi1_8w1byte((uint8_t)(MEM_LOCATION_24 >> (8 * (2 - i)))); // Send 24bit memroy adress with MSB first
    }
    int8_t DATA = spi1_8r1byte(); // Data at the mem location

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave device

    return DATA; // Return the meomery data
}

void w25q32_w1byte(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_BLOCK, uint32_t MEM_LOCATION_24, int8_t DATA) // Flash memory write 1byte
{
    // Write enable
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave device
    spi1_8w1byte(0x06);                                // Write enable cmd byte
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave device

    // Page program
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave device
    spi1_8w1byte(0x02);                                // Page programm cmd
    for (volatile int i = 0; i < 3; i++)
    {
        spi1_8w1byte((uint8_t)(MEM_LOCATION_24 >> (8 * (2 - i)))); // Send 24bit memroy adress with MSB first
    }
    spi1_8w1byte(DATA);                                // Send the byte to write
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave device

    // Status Register 1 check
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 1); // Select slave device
    spi1_8w1byte(0x05);                                // Read Status Register 1 cmd
    uint8_t temp;
    while (1) // Wait till the register write operation is finished
    {
        temp = spi1_8r1byte(); // Read status register values

        if (!(temp & 1)) // Check if the busy flag is cleared
            break;
    }
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_BLOCK, 0); // De-select slave device
}