#include "EALGO.h"
#include "../W25Q32_MS/W25Q32.h"

static EALGO ealgo;

void ealga_logset(uint32_t LOG_START, uint32_t LOG_END)
{
    ealgo.LOG_START = LOG_START;
    ealgo.LOG_END = LOG_END;
}

void ealga_dataset(uint32_t DATA_START, uint32_t DATA_END)
{
    ealgo.DATA_START = DATA_START;
    ealgo.DATA_END = DATA_END;
}

void ealga_boot()
{
    uint8_t flag = 0;
    uint32_t log_addr = 0;
    uint8_t log = 0;
    while (!flag)
    {
        log_addr = (ealgo.LOG_START + ealgo.LOG_END) / 2;

        w25q32_read(log_addr, log, 8);

        if (log == 1)
            flag = 1;
    }
}
/*
ealga_init(){

find flags
use sorting algorithm(point = point/2)

ones last updated flag is found read its data and save last write addr
if write action corrupted clear the data at that last addr by over writing it
now use the addr to save data form now on for both log and data

}
*/

/*
ealga_save(){
set CS flag
set WS flag
save the data to the wrie addr
incriment and also upade the write flag addr space after write action and incriment the addr
set WE flag
}
*/

/*
ealga_edge(){
set RS flag
read from the last written adress to back from required amount of addr.
set RE flag
set CE flag

use the data to calculate the required data using proper methods
output the data to be displayed
}
*/