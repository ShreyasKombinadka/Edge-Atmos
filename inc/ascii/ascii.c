#include "ascii.h"
#include <stdint.h>

void num_00_00_ascii(float num, uint8_t *num_ascii)
{
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

    if (count != 0 && num_temp == 0)
    {
        while (count >= 0)
        {
            if (count == (2))
                num_ascii[count] = 0x2E;
            else
            {
                num_ascii[count] = 0x30;
            }

            count--;
        }
    }

    num_ascii[5] = '\0';
}
