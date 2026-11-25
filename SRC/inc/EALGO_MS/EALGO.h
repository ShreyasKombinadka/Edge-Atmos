#ifndef EALGO_H
#define EALGO_H

#include <stdint.h>

typedef struct
{
    uint32_t DATA_START;
    uint32_t DATA_END;

    uint32_t LOG_START;
    uint32_t LOG_END;
} EALGO;

void ealgo_logset(uint32_t LOG_START, uint32_t LOG_END);    // Log location set
void ealgo_dataset(uint32_t DATA_START, uint32_t DATA_END); // Data location set
void ealgo_boot();

#endif