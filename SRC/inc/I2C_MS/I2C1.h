#ifndef I2C1_H
#define I2C1_H

#include <stdint.h>

typedef enum // States of I2C sequnece
{
    START,
    SB,
    ADDR,
    WRITE,
    TxE,
    BTF,
    STOP
} I2C_STATE;

typedef struct
{
    I2C_STATE STATE;  // I2C state tracker
    uint8_t ADDR;     // Slave address
    uint8_t *W_DATA;  // Write data
    uint8_t W_COUNT;  // Write byte counter
    uint8_t W_LENGTH; // Write byte number
    uint8_t N_BYTE;   // N byte flag
} I2C;

void i2c1_init(void);
void i2c1_wake(uint8_t ADDR);
void i2c1_w1byte(uint8_t DATA);
void i2c1_stop(void);
void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n);

void I2C1_IRQHandler(void); // Interupt handler

#endif