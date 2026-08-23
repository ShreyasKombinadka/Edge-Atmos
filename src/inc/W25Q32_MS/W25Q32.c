#include "W25Q32.h"
#include "../SPI_MS/SPI1.h"
#include <stdint.h>

void w25q32_read(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint32_t MEM_LOCATION_24, uint8_t *DATA, uint8_t DATA_SIZE_BYTE) // Flash memory read
{
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device

    spi1_8w1byte(0x03); // Read enable cmd byte
    for (volatile int i = 0; i < 3; i++)
        spi1_8w1byte((uint8_t)(MEM_LOCATION_24 >> (8 * (2 - i)))); // Send 24bit memroy adress with MSB first

    // Read data
    volatile uint8_t byte_count = 0;
    while (byte_count < DATA_SIZE_BYTE)
    {
        DATA[byte_count] = spi1_8r1byte();
        byte_count++;
    }

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device
}

void w25q32_write(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint32_t MEM_LOCATION_24, uint8_t *DATA, uint8_t DATA_SIZE_BYTE) // Flash memory write
{
    volatile uint8_t byte_count = 0;  // Bytes sent
    volatile uint8_t page_switch = 0; // Page switch flag
    while (byte_count < DATA_SIZE_BYTE)
    {
        if (byte_count == 0 || page_switch == 1)
        {
            page_switch = 0; // Clear page switch flag

            // Write enable
            spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device
            spi1_8w1byte(0x06);                               // Write enable cmd byte
            spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device

            // Page program
            spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device
            spi1_8w1byte(0x02);                               // Page programm cmd
            for (volatile int i = 0; i < 3; i++)
                if (byte_count == 0)
                    spi1_8w1byte((uint8_t)((MEM_LOCATION_24) >> (8 * (2 - i)))); // Send 24bit memroy adress with MSB first
                else
                    spi1_8w1byte((uint8_t)((MEM_LOCATION_24 + byte_count) >> (8 * (2 - i)))); // Send 24bit memroy adress of next page with MSB first
        }

        spi1_8w1byte(DATA[byte_count]); // Send the byte to write
        if (((byte_count + MEM_LOCATION_24) & 0xFF) >= 0xFF)
            page_switch = 1;
        byte_count++;

        if (page_switch == 1 || byte_count >= DATA_SIZE_BYTE)
        {
            spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device
            // Status Register 1 check
            spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device
            spi1_8w1byte(0x05);                               // Read Status Register 1 cmd
            uint8_t temp;
            while (1) // Wait till the register write operation is finished
            {
                temp = spi1_8r1byte(); // Read status register values

                if (!(temp & 1)) // Check if the busy flag is cleared
                    break;
            }

            spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device
        }
    }

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device
}

void w25q32_sectorclear(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint32_t MEM_LOCATION_24) // Sector clear
{
    // Write enable
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device
    spi1_8w1byte(0x06);                               // Write enable cmd byte
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device

    // Sector clear
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device
    spi1_8w1byte(0x20);                               // Sector clear cmd byte
    for (volatile int i = 0; i < 3; i++)
        spi1_8w1byte((uint8_t)(MEM_LOCATION_24 >> (8 * (2 - i)))); // Send 24bit memroy adress with MSB first
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0);              // De-select slave device

    // Status Register 1 check
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device
    spi1_8w1byte(0x05);                               // Read Status Register 1 cmd
    uint8_t temp;
    while (1) // Wait till the register write operation is finished
    {
        temp = spi1_8r1byte(); // Read status register values

        if (!(temp & 1)) // Check if the busy flag is cleared
            break;
    }
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device
}

void w25q32_jedec(uint8_t SLAVE_CS, uint8_t SLAVE_CS_PIN_PORT, uint8_t *DATA) // JEDEC ID
{
    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 1); // Select slave device

    spi1_8w1byte(0x9F); // JEDEC ID cmd
    // Read data
    for (volatile int i = 0; i < 3; i++) // Read 3 bytes
        DATA[i] = spi1_8r1byte();

    spi1_slaveselect(SLAVE_CS, SLAVE_CS_PIN_PORT, 0); // De-select slave device
}