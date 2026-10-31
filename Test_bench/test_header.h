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
    I2C_STATE STATE;       // I2C state tracker
    uint8_t ADDR;          // Slave address
    uint8_t ADDR_BUFF[32]; // Addr buffer
    uint8_t R_ABUFF_ADDR;  // Addr buffer read location
    uint8_t W_ABUFF_ADDR;  // Data buffer write location
    uint8_t DATA_BUFF[64]; // Data buffer
    uint8_t R_DBUFF_ADDR;  // Data buffer read location
    uint8_t W_DBUFF_ADDR;  // Data buffer write location
    uint8_t END_BUFF[32];  // End buffer
    uint8_t R_EBUFF_ADDR;  // End buffer read location
    uint8_t W_EBUFF_ADDR;  // End buffer write location
    uint8_t N_BYTE;        // N byte flag
} I2C;

#define BUFF_SIZE 5

void i2c1_init(void);
void i2c1_wake(uint8_t ADDR);
void i2c1_w1byte(uint8_t DATA);
void i2c1_stop(void);
void i2c1_rsnbyte(uint8_t addr, uint8_t *data_addr, int n);
void buff_rst(uint8_t BUFF_SELECT);                 // Buffer reset
void buff_write(uint8_t BUFF_SELECT, uint8_t DATA); // Buffer write
uint8_t buff_read(uint8_t BUFF_SELECT);             // Buffer read

void I2C1_IRQHandler(void); // Interupt handler

#endif