#include <stdint.h>
#include <stdio.h>
#include "test_header.h"

int main(void)
{
    buff_rst(0);
    buff_rst(1);
    buff_rst(2);
    buff_rst(3);

    printf("Done\n");
    return 0;
}