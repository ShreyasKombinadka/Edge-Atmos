#ifndef I2C_H
#define I2C_H

#include <stdint.h>

struct I2C
{
    uint8_t FREQ;
    uint8_t CCR;
    uint8_t TRISE;
    uint16_t DR;
};

void I2C1_start(void);
void I2C1_write(uint8_t addr, uint8_t data);
uint8_t I2C1_read1b(uint8_t addr);
uint8_t I2C1_read2b(uint8_t addr);
void I2C1_stop(void);

#endif