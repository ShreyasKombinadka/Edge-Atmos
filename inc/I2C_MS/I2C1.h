#ifndef I2C1_H
#define I2C1_H

#include <stdint.h>

void i2c1_init(void);
void i2c1_wake(uint8_t addr);
void i2c1_stop(void);
void i2c1_w1byte(uint8_t data);
void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n);

#endif