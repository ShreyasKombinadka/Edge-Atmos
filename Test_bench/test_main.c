#include <stdint.h>
#include <stdio.h>
#include "test_header.h"

#define BUFF_SIZE 5

int main(void)
{
    // Reset
    printf("\nReset\n");
    buff_rst(0);
    buff_rst(1);
    buff_rst(2);
    buff_rst(3);

    // Read
    printf("\nRead\n");
    uint8_t buff = 0;
    buff = buff_read(0);
    printf("Value : %d\n", (int)buff);
    buff = buff_read(1);
    printf("Value : %d\n", (int)buff);
    buff = buff_read(2);
    printf("Value : %d\n", (int)buff);
    buff = buff_read(0);
    printf("Value : %d\n", (int)buff);

    // Continues read
    printf("\nContinues read\n");
    for (volatile int i = 0; i < BUFF_SIZE; i++)
    {
        buff = buff_read(0);
        printf("Value : %d\n", (int)buff);
    }

    // Reset
    printf("\nReset\n");
    buff_rst(0);
    buff_rst(1);
    buff_rst(2);
    buff_rst(3);

    // Continues write
    printf("\nContinues write\n");
    for (volatile int i = 0; i < BUFF_SIZE; i++)
    {
        buff_write(0, 69);
    }

    // Write and read
    printf("\nContinues write & read\n");
    buff_write(0, 96);
    buff = buff_read(0);
    printf("Value : %d\n", (int)buff);

    // Continues read
    printf("\nContinues read\n");
    for (volatile int i = 0; i < BUFF_SIZE; i++)
    {
        buff = buff_read(0);
        printf("Value : %d\n", (int)buff);
    }

    printf("DONE..!\n");
    return 0;
}