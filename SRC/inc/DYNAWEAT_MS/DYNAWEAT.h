#ifndef DYNAWEAT_H
#define DYNAWEAT_H

#include <stdint.h>

// Normal temperature range in Celcius
#define NORM_TEMP_HIGH 32 // Higher limit
#define NORM_TEMP_LOW 18  // Lower limit

// Normal presure range in hpa
#define NORM_PRES_HIGH 910 // Higher limit
#define NORM_PRES_LOW 880  // Lower limit

// Normal humidity range in percentage
#define NORM_HUMI_HIGH 70 // Higher limit
#define NORM_HUMI_LOW 30  // Lower limit

uint16_t dynaweat_temp16(float TEMPERATURE); // Dynamic temperature color indicator
uint16_t dynaweat_pres16(float PRESURE);     // Dynamic presure color indicator
uint16_t dynaweat_humi16(float HUMIDITY);    // Dynamic humidity color indicator

#endif