#include <stdint.h>
#include <stdio.h>
#include "test_header.h"

// TEST BLOCK

static I2C i2c1;

// Helper functions
void buff_rst(uint8_t BUFF_SELECT) // Buffer reset
{
    switch (BUFF_SELECT)
    {
    case 0:                                          // Addr buffer
        i2c1.W_ABUFF_ADDR = 0;                       // Addr buff write addr clear
        i2c1.R_ABUFF_ADDR = 0;                       // Addr buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Addr buff clear
            i2c1.ADDR_BUFF[i] = 0;
        break;
    case 1:                                          // Data buffer
        i2c1.W_DBUFF_ADDR = 0;                       // Data buff write addr clear
        i2c1.R_DBUFF_ADDR = 0;                       // Data buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Data buff clear
            i2c1.DATA_BUFF[i] = 0;
        break;
    case 2:                                          // End buffer
        i2c1.W_EBUFF_ADDR = 0;                       // End buff write addr clear
        i2c1.R_EBUFF_ADDR = 0;                       // End buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // End buff clear
            i2c1.END_BUFF[i] = 0;
        break;
    case 3: // All buffer reset
        // Addr buffer
        i2c1.W_ABUFF_ADDR = 0;                       // Addr buff write addr clear
        i2c1.R_ABUFF_ADDR = 0;                       // Addr buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Addr buff clear
            i2c1.ADDR_BUFF[i] = 0;

        // Data buffer
        i2c1.W_DBUFF_ADDR = 0;                       // Data buff write addr clear
        i2c1.R_DBUFF_ADDR = 0;                       // Data buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // Data buff clear
            i2c1.DATA_BUFF[i] = 0;

        // End buffer
        i2c1.W_EBUFF_ADDR = 0;                       // End buff write addr clear
        i2c1.R_EBUFF_ADDR = 0;                       // End buff read addr clear
        for (volatile int i = 0; i < BUFF_SIZE; i++) // End buff clear
            i2c1.END_BUFF[i] = 0;
        break;
    default:
        break;
    }

    // Debug print
    printf("Reset : %d\n", BUFF_SELECT);
}

void buff_write(uint8_t BUFF_SELECT, uint8_t DATA) // Buffer write
{
    switch (BUFF_SELECT)
    {
    case 0: // Addr buffer
        if (i2c1.W_ABUFF_ADDR > BUFF_SIZE - 1)
            i2c1.W_ABUFF_ADDR = 0; // Buff write addr reset
        i2c1.ADDR_BUFF[i2c1.W_ABUFF_ADDR] = DATA;
        i2c1.W_ABUFF_ADDR++; // Incrimnet buff location
        break;
    case 1: // Data buffer
        if (i2c1.W_DBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.W_DBUFF_ADDR = 0; // Buff write addr reset
        i2c1.DATA_BUFF[i2c1.W_DBUFF_ADDR] = DATA;
        i2c1.W_DBUFF_ADDR++; // Incrimnet buff location
        break;
    case 2: // End buffer
        if (i2c1.W_EBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.W_EBUFF_ADDR = 0; // Buff write addr reset
        i2c1.END_BUFF[i2c1.W_EBUFF_ADDR] = DATA;
        i2c1.W_EBUFF_ADDR++; // Incrimnet buff location
        break;
    default:
        break;
    }

    // Debug print
    printf("Write : %d\n", BUFF_SELECT);
    printf("Buff write locations - AW:%d DW:%d EW:%d \n", i2c1.W_ABUFF_ADDR - 1, i2c1.W_DBUFF_ADDR - 1, i2c1.W_EBUFF_ADDR - 1);
}

uint8_t buff_read(uint8_t BUFF_SELECT) // Buffer read
{
    uint8_t data_temp = 0; // Temp data varibale <<<<<<<<<<<<<<<<<<<<<<<<<<=============================== Needs to be intigrated to main code
    switch (BUFF_SELECT)
    {
    case 0: // Addr buffer
        if (i2c1.R_ABUFF_ADDR > BUFF_SIZE - 1)
            i2c1.R_ABUFF_ADDR = 0; // Buff read addr reset
        data_temp = i2c1.ADDR_BUFF[i2c1.R_ABUFF_ADDR];
        i2c1.R_ABUFF_ADDR++; // Incrimnet buff location
        break;
    case 1: // Data buffer
        if (i2c1.R_DBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.R_DBUFF_ADDR = 0; // Buff read addr reset
        data_temp = i2c1.DATA_BUFF[i2c1.R_DBUFF_ADDR];
        i2c1.R_DBUFF_ADDR++; // Incrimnet buff location
        break;
    case 2: // End buffer
        if (i2c1.R_EBUFF_ADDR > BUFF_SIZE - 1)
            i2c1.R_EBUFF_ADDR = 0; // Buff read addr reset
        data_temp = i2c1.END_BUFF[i2c1.R_EBUFF_ADDR];
        i2c1.R_EBUFF_ADDR++; // Incrimnet buff location
        break;
    default:
        break;
    }

    // Debug print
    printf("Read : %d\n", BUFF_SELECT);
    printf("Buff read locations - AR:%d DR:%d ER:%d \n", i2c1.R_ABUFF_ADDR - 1, i2c1.R_DBUFF_ADDR - 1, i2c1.R_EBUFF_ADDR - 1);

    return data_temp; // Return data
}