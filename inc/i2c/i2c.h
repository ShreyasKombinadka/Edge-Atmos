#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c1_init(void);
void i2c1_wake(uint8_t addr);
void i2c1_stop(void);
void i2c1_w1byte(uint8_t data);
void i2c1_wws1byte(uint8_t addr, uint8_t data);
uint8_t i2c1_rs1byte(uint8_t addr);
uint16_t i2c1_rs2byte(uint8_t addr);
void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n);

#endif