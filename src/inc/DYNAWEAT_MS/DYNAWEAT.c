#include "DYNAWEAT.h"
#include "../COLOR_MS/COLOR.h"
#include <stdint.h>

uint16_t dynaweat_temp(float TEMPERATURE)
{
    if (TEMPERATURE > NORM_TEMP_HIGH)
        return MS_BRIGHT_RED_16;

    else if (TEMPERATURE < NORM_TEMP_HIGH && TEMPERATURE > NORM_TEMP_LOW)
        return MS_ORANGE_16;

    if (TEMPERATURE < NORM_TEMP_LOW)
        return MS_BRIGHT_CYAN_16;
}
