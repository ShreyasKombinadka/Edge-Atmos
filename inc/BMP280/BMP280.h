#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>

void bmp280_init(void);
void bmp280_read(float *temp, float *humi);

#endif

/* I2C BMP280 Sensor Protocol Specification

Device I2C Address: 0x76 or 0x77 (Depending on SDO pin state)

================================================================================
                         DATA PACKET STREAM FORMAT
================================================================================
A continuous 6-byte burst read block is fetched sequentially starting from
the Press MSB register address (0xF7).

Byte 1: Pressure Data MSB    [Bits 19–12] (Register 0xF7)
Byte 2: Pressure Data LSB    [Bits 11–4]  (Register 0xF8)
Byte 3: Pressure Data XLSB   [Bits 3–0]   (Register 0xF9, upper 4 bits)
Byte 4: Temperature Data MSB [Bits 19–12] (Register 0xFA)
Byte 5: Temperature Data LSB [Bits 11–4]  (Register 0xFB)
Byte 6: Temperature Data XLSB[Bits 3–0]   (Register 0xFC, upper 4 bits)


================================================================================
                            OPERATIONAL COMMANDS
================================================================================
NOTE : This is for low accurasy low power mode.

1. Reset Packet (Soft Reset)
   - Write Sequence : Write 0xB6 to Register 0xE0
   - Timing       : Send once at startup if needed. Wait 2ms for complete boot.
   - Function     : Triggers a complete soft reset identical to power-on reset.

2. Configuration Packet (ctrl_meas)
   - Write Sequence : Write 0x2E to Register 0xF4 (Example configuration)
   - Timing       : Configure once at startup.
   - Function     : Bits [7:5] (001) = Temp oversampling x1.
                    Bits [4:2] (011) = Pressure oversampling x4.
                    Bits [1:0] (10)  = Forced mode operation (starts conversion).
                    Note: In forced mode, sensor automatically drops back into
                    sleep state after measurement cycle finishes.

3. Data Retrieval Request
   - Action       : Standard I2C Master burst read request for 6 bytes starting
                    from register address 0xF7.
   - Timing       : Execute after measurement cycle finishes (approx. 10ms–15ms
                    depending on oversampling settings).
   - Function     : Continuously auto-increments internal register pointers to
                    grab raw 20-bit pressure and 20-bit temperature packets.


================================================================================
                         DATA PARSING & CONVERSION
================================================================================
Temperature Bit Alignment:
- Take Byte 4 to form the highest bits [19:12] of the temperature.
- Append Byte 5 to form the middle bits [11:4].
- Append the upper 4 bits (Bits 7–4) of Byte 6 to form the lowest bits [3:0].
- This creates the final 20-bit raw integer value.

Pressure Bit Alignment:
- Take Byte 1 to form the highest bits [19:12] of the pressure.
- Append Byte 2 to form the middle bits [11:4].
- Append the upper 4 bits (Bits 7–4) of Byte 3 to form the lowest bits [3:0].
- This creates the final 20-bit raw integer value.

Mathematical Formula:
Unlike simpler sensors, the BMP280 requires 20-bit raw values to be passed through
factory calibration trimming parameters (dig_T1..dig_T3 and dig_P1..dig_P9)
stored in registers 0x88 to 0xA1.

                     Raw_Value passed to Bosch API equations
     Final Result -> yields compensated Temperature (°C) and Pressure (Pa).
*/