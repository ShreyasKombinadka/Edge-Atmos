#ifndef I2C_H
#define I2C_H

#include <stdint.h>

struct I2C
{
    uint8_t FREQ;
    uint8_t CCR;
    uint8_t TRISE;
};

void I2C1_start(void);
void I2C1_wake(uint8_t addr);
void I2C1_wws1bint(uint8_t addr, uint8_t data);
void I2C1_w1bint(uint8_t data);
uint8_t I2C1_rs1bint(uint8_t addr);
uint16_t I2C1_rs2bint(uint8_t addr);
void I2C1_stop(void);

#endif