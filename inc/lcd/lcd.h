#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init(uint8_t addr);
void lcd_w1byte(uint8_t data, uint8_t rs);
void lcd_wwscmd(uint8_t addr, uint8_t cmd);
void lcd_wws1byte(uint8_t addr, uint8_t data);
void lcd_wws8byte(uint8_t addr, uint64_t data, int n);
void lcd_wwsstring(uint8_t addr, uint8_t *char_addr);
void lcd_debug(uint8_t addr, uint8_t *char_addr);
void lcd_clear(uint8_t addr);
void lcd_row2(uint8_t addr);

#endif

/* STM32 -> PCF8574 -> LCD1602

PCF8574 Pin Mapping to LCD:
MSB                                      LSB
+----+----+----+----+----+---+----+----+
| D7 | D6 | D5 | D4 | BT | E | RW | RS |
+----+----+----+----+----+---+----+----+

Bit 7 (MSB): D7 - LCD Data Bit 7 (Highest)
Bit 6      : D6 - LCD Data Bit 6
Bit 5      : D5 - LCD Data Bit 5
Bit 4      : D4 - LCD Data Bit 4 (Lowest)
Bit 3      : BT - Backlight (1 = ON, 0 = OFF)
Bit 2      : E  - Enable Clock Pulse (1 = HIGH, 0 = LOW)
Bit 1      : RW - Read/Write (0 = Write, 1 = Read)
Bit 0 (LSB): RS - Register Select (0 = Command, 1 = Data)

================================================================================
                           MAIN OPERATIONAL COMMANDS
================================================================================
[0x01] Clear Display
       - Clears all text and resets cursor to top-left.
       - Requires a long 2ms delay after sending.

[0x02] Return Home
       - Returns cursor to top-left without clearing text.
       - Requires a 2ms delay.

[0x06] Entry Mode Set
       - Configures text direction.
       - Automatically moves cursor right after each character, no display shift.

[0x0C] Display ON
       - Turns display screen ON, hides the block cursor, hides cursor blinking.

[0x0E] Cursor ON
       - Turns display screen ON, shows underline block cursor, hides blinking.

[0x0F] Blinking ON
       - Turns display screen ON, shows underline block cursor, enables blinking.

[0x08] Display OFF
       - Blanks the screen entirely. Text data remains intact in memory.

[0x14] Move Cursor Right
       - Shifts the cursor position one step to the right manually.

[0x10] Move Cursor Left
       - Shifts the cursor position one step to the left manually.

[0x28] Function Set
       - Configures hardware interface: 4-bit mode, 2 lines matrix, 5x8 font size.

[0xC0] Row 2, Column 1 Address
       - Positions the text cursor at the start of the second row.


================================================================================
                    CURSOR POSITION / ROW SELECTION COMMANDS
================================================================================
To position the text cursor at a specific row and column, send a single command
calculated as: 0x80 + DDRAM_Address

Base Row Addresses:
- Row 1, Column 1: 0x80
- Row 2, Column 1: 0xC0

Targeting an exact Column (X) on a standard 16x2 Display (where X is 0 to 15):
- Row 1: 0x80 + X
- Row 2: 0xC0 + X


================================================================================
              EX: Protocol to Send a Command Byte (e.g., 0x35)
================================================================================
Assuming: Backlight = ON (1), RW = Write (0), RS = Command (0)
Control Bits state: [ BT=1, E=?, RW=0, RS=0 ]

1. Send Upper Nibble (0x3) shifted left -> 0x30. Turn Enable HIGH.
   Bits:  [ 0 0 1 1 ] [ 1 1 0 0 ]  --> Hex: 0x3C
   Delay: Wait 1 microsecond

2. Send Upper Nibble (0x30). Turn Enable LOW to latch data.
   Bits:  [ 0 0 1 1 ] [ 1 0 0 0 ]  --> Hex: 0x38
   Delay: Wait 1 microsecond

3. Send Lower Nibble (0x5) shifted left -> 0x50. Turn Enable HIGH.
   Bits:  [ 0 1 0 1 ] [ 1 1 0 0 ]  --> Hex: 0x5C
   Delay: Wait 1 microsecond

4. Send Lower Nibble (0x50). Turn Enable LOW to latch data.
   Bits:  [ 0 1 0 1 ] [ 1 0 0 0 ]  --> Hex: 0x58
   Delay: Wait 50 microseconds (Execution time)
*/