#include "ascii.h"
#include <stdint.h>

void num_uint_ascii(unsigned int num, uint8_t *num_ascii)
{
    unsigned int num_temp = num;
    int count = 0;

    if (num_temp > 0)
    {
        while (num_temp > 0)
        {
            num_ascii[count] = (num_temp % 10) + '0';
            num_temp /= 10;
            count++;
        }

        if (count >= 2)
        {
            int8_t temp;
            for (volatile int i = 0; i <= ((count - 1.0) / 2); i++)
            {
                temp = num_ascii[(count - 1) - i];
                num_ascii[(count - 1) - i] = num_ascii[i];
                num_ascii[i] = temp;
            }
        }
    }
    else
    {
        num_ascii[0] = '0';
        count++;
    }

    num_ascii[count] = '\0';
}

void num_4digi_ascii(float num, uint8_t *num_ascii)
{
    unsigned int num_temp = 0;
    int point = 0;
    if (num >= 1000 && num < 10000)
    {
        num_temp = num;
    }
    else if (num >= 100)
    {
        point = 3;
        num_temp = num * 10;
    }
    else if (num >= 10)
    {
        point = 2;
        num_temp = num * 100;
    }
    else if (num >= 1)
    {
        point = 1;
        num_temp = num * 1000;
    }
    else if (num >= 0.01)
    {
        num_temp = num * 1000;
    }
    else
        num_temp = 0;

    if (point == 0 && num_temp == 0)
    {
        int count = 4;
        while (count >= 0)
        {
            if (count == 2)
                num_ascii[count] = 0x2E;
            else
            {
                num_ascii[count] = 0x30;
            }

            count--;
        }
    }

    else if (point == 0 && num < 1 && num >= 0.01)
    {
        uint8_t arr_temp[4];
        int count = 0;
        while (num_temp > 0 && count < 4)
        {
            arr_temp[count] = (num_temp % 10) + '0';
            num_temp /= 10;

            count++;
        }

        if (count < 4)
        {
            int pad_count = count;
            while (pad_count < 4)
            {
                arr_temp[pad_count] = 0x30;
                pad_count++;
            }
        }

        num_ascii[0] = '0';
        num_ascii[1] = 0x2E;
        int lmt;
        if (num >= 0.1)
        {
            for (volatile int i = 0; i < 3; i++)
            {
                num_ascii[i + 2] = arr_temp[count - 1 - i];
            }
        }
        else
        {
            num_ascii[2] = '0';
            for (volatile int i = 0; i < 2; i++)
            {
                num_ascii[i + 3] = arr_temp[count - 1 - i];
            }
        }
    }

    else if (num >= 1000 && num < 10000)
    {
        num_uint_ascii(num_temp, num_ascii);
    }
    else
    {
        int count = 4;
        while (num_temp > 0)
        {
            if (count == point && point > 0)
                num_ascii[count] = 0x2E;
            else
            {
                num_ascii[count] = (num_temp % 10) + '0';
                num_temp /= 10;
            }

            count--;
        }

        while (count >= 0)
        {
            if (count == point && point > 0)
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
