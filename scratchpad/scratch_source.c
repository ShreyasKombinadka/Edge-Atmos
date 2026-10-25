#include <stdint.h>
#include <stdio.h>
#include "scratch_header.h"

static I2C i2c1;
// Helper functions
void buff_rst(uint8_t BUFF_SELECT) // Buffer reset
{
    switch (BUFF_SELECT)
    {
    case 0:
        i2c1.W_ABUFF_ADDR = 0; // Buff write addr clear
        i2c1.R_ABUFF_ADDR = 0; // Buff read addr clear

        for (volatile int i = 0; i < BUFF_SIZE; i++)
            i2c1.ADDR_BUFF[i] = 0;
        break;
    case 1:
        i2c1.W_DBUFF_ADDR = 0; // Buff write addr clear
        i2c1.R_DBUFF_ADDR = 0; // Buff read addr clear

        for (volatile int i = 0; i < BUFF_SIZE; i++)
            i2c1.DATA_BUFF[i] = 0;
        break;
    case 2:
        i2c1.W_EBUFF_ADDR = 0; // Buff write addr clear
        i2c1.R_EBUFF_ADDR = 0; // Buff read addr clear

        for (volatile int i = 0; i < BUFF_SIZE; i++)
            i2c1.END_BUFF[i] = 0;
        break;
    default:
        break;
    }
}

void buff_write(uint8_t BUFF_SELECT, uint8_t DATA) // Buffer write
{
    switch (BUFF_SELECT)
    {
    case 0:
        if (i2c1.W_ABUFF_ADDR >= BUFF_SIZE)
            i2c1.W_ABUFF_ADDR = 0; // Buff write addr reset

        i2c1.ADDR_BUFF[i2c1.W_ABUFF_ADDR] = DATA;
        i2c1.W_ABUFF_ADDR++;
        break;
    case 1:
        if (i2c1.W_DBUFF_ADDR >= BUFF_SIZE)
            i2c1.W_DBUFF_ADDR = 0; // Buff write addr reset

        i2c1.DATA_BUFF[i2c1.W_DBUFF_ADDR] = DATA;
        i2c1.W_DBUFF_ADDR++;
        break;
    case 2:
        if (i2c1.W_EBUFF_ADDR >= BUFF_SIZE)
            i2c1.W_EBUFF_ADDR = 0; // Buff write addr reset

        i2c1.END_BUFF[i2c1.W_EBUFF_ADDR] = DATA;
        i2c1.W_EBUFF_ADDR++;
        break;
    default:
        break;
    }
}

uint8_t buff_read(uint8_t BUFF_SELECT) // Buffer read
{
    switch (BUFF_SELECT)
    {
    case 0:
        if (i2c1.R_ABUFF_ADDR >= BUFF_SIZE)
            i2c1.R_ABUFF_ADDR = 0; // Buff read addr reset

        i2c1.R_ABUFF_ADDR++;
        return i2c1.ADDR_BUFF[i2c1.R_ABUFF_ADDR - 1];
        break;
    case 1:
        if (i2c1.R_DBUFF_ADDR >= BUFF_SIZE)
            i2c1.R_DBUFF_ADDR = 0; // Buff read addr reset

        i2c1.R_DBUFF_ADDR++;
        return i2c1.DATA_BUFF[i2c1.R_DBUFF_ADDR - 1];
        break;
    case 2:
        if (i2c1.R_EBUFF_ADDR >= BUFF_SIZE)
            i2c1.R_EBUFF_ADDR = 0; // Buff read addr reset

        i2c1.R_EBUFF_ADDR++;
        return i2c1.END_BUFF[i2c1.R_EBUFF_ADDR - 1];
        break;
    default:
        break;
    }
}