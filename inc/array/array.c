#include "array.h"
#include <stdint.h>

void array_invert(uint8_t *arr_addr, int arr_size)
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