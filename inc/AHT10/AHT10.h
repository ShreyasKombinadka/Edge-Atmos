#ifndef AHT10_H
#define AHT10_H

#include <stdint.h>

void aht10_init(void);
void aht10_read(float *temp, float *humi);

#endif

/* I2C AHT10 Sensor Protocol Specification

Device I2C Address: 0x38 (7-bit)

================================================================================
                         DATA PACKET STREAM FORMAT
================================================================================
A continuous 6-byte read block must be fetched sequentially from the device.

Byte 1: Status Byte (Ignore)
Byte 2: Humidity Data [Bits 19–12]
Byte 3: Humidity Data [Bits 11–4]
Byte 4: Mixed Byte    --> [Bits 3–0 of Humidity] + [Bits 19–16 of Temperature]
Byte 5: Temp Data     --> [Bits 15–8 of Temperature]
Byte 6: Temp Data     --> [Bits 7–0 of Temperature]


================================================================================
                            OPERATIONAL COMMANDS
================================================================================
1. Initialization Packet
   - Hex Sequence : 0xE1, 0x08, 0x00
   - Timing       : Send once at startup. Wait 20ms immediately after sending.
   - Function     : 0xE1 is the initialization command. 0x08 forces the internal
                    state machine to load the factory calibration coefficients
                    into the configuration register. 0x00 completes alignment.
                    If omitted, the sensor returns invalid data.

2. Measurement Trigger Packet
   - Hex Sequence : 0xAC, 0x33, 0x00
   - Timing       : Send every time you want a new reading. Wait 75ms
                    immediately after sending.
   - Function     : 0xAC is the explicit trigger opcode that wakes the sensor core.
                    0x33 configures the internal ADC resolution for a combined
                    humidity/temperature conversion cycle. 0x00 clears the
                    remaining command buffer to execute the request.

3. Data Retrieval Request
   - Action       : Standard I2C Master Read request for 6 bytes from address 0x38.
   - Timing       : Execute only after the 75ms measurement delay completes.
   - Function     : Fetches the raw data payload containing the 20-bit status,
                    humidity, and temperature values.


================================================================================
                         DATA PARSING & CONVERSION
================================================================================
Temperature Bit Alignment:
- Take the lower 4 bits of Byte 4 to form the highest bits of the temperature.
- Append Byte 5 to form the middle bits.
- Append Byte 6 to form the lowest bits.
- This creates the final 20-bit raw integer value.

Mathematical Formula:
                                 (Raw_Value * 200)
            Temperature (°C)  =  -------------------  -  50
                                    1048576

                                 (Raw_Value * 100)
      Relative Humidity (%)   = -------------------
                                    1048576
*/