#include "ascii.h"
#include <stdint.h>

uint8_t *num_to_ascii(float num)
{
    static uint8_t num_ascii[5];
    int num_temp = num * 100;

    int count = 4;
    while (num_temp > 0)
    {
        if (count == (2))
            num_ascii[count] = 0x2E;
        else
        {
            num_ascii[count] = (num_temp % 10) + '0';
            num_temp /= 10;
        }

        count--;
    }

    return num_ascii;
}