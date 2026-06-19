#include "array.h"
#include <stdint.h>

void arr_inv(uint8_t *arr_addr, int arr_size)
{
    if (arr_size >= 2)
    {
        int64_t temp;
        for (volatile int i = 0; i < ((arr_size - 1.0) / 2); i++)
        {
            temp = arr_addr[(arr_size - 1) - i];
            arr_addr[(arr_size - 1) - i] = arr_addr[i];
            arr_addr[i] = temp;
        }
    }
}

void arr_rshift(uint8_t *arr_addr, int arr_size)
{
    if (arr_size >= 2)
    {
        for (volatile int i = (arr_size - 1); i > 0; i--)
        {
            arr_addr[i] = arr_addr[i - 1];
        }

        arr_addr[0] = '0';
    }
}