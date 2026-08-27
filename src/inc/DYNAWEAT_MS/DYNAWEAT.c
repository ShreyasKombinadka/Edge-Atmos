#include "DYNAWEAT.h"
#include "../COLOR_MS/COLOR.h"
#include <stdint.h>

uint16_t dynaweat_temp16(float TEMPERATURE) // Dynamic temperature color indicator
{
    if (TEMPERATURE > NORM_TEMP_HIGH) // Above the normal limit
        return MS_BRIGHT_RED_16;

    else if (TEMPERATURE < NORM_TEMP_HIGH && TEMPERATURE > NORM_TEMP_LOW) // Normal range
        return MS_ORANGE_16;

    if (TEMPERATURE < NORM_TEMP_LOW) // Below the normal limit
        return MS_BRIGHT_CYAN_16;
}

uint16_t dynaweat_pres16(float PRESURE) // Dynamic presure color indicator
{
    if (PRESURE > NORM_PRES_HIGH) // Above the normal limit
        return MS_BRIGHT_RED_16;

    else if (PRESURE < NORM_PRES_HIGH && PRESURE > NORM_PRES_LOW) // Normal range
        return MS_GREEN_16;

    if (PRESURE < NORM_PRES_LOW) // Below the normal limit
        return MS_BRIGHT_CYAN_16;
}

uint16_t dynaweat_humi16(float HUMIDITY) // Dynamic humidity color indicator
{
    if (HUMIDITY > NORM_HUMI_HIGH) // Above the normal limit
        return MS_BLUE_16;

    else if (HUMIDITY < NORM_HUMI_HIGH && HUMIDITY > NORM_HUMI_LOW) // Normal range
        return MS_BRIGHT_BLUE_16;

    if (HUMIDITY < NORM_HUMI_LOW) // Below the normal limit
        return MS_BRIGHT_CYAN_16;
}