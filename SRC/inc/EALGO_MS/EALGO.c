#include "EALGO.h"

void ealga_init() {}
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