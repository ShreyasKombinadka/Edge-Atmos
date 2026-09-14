#include "ST7789LCD.h"
#include "../SPI_MS/SPI1.h"
#include "../GPIO_MS/GPIO.h"
#include "../FONT_MS/BITMAP.h"

#include <stdint.h>

static struct ST7789LCD st7789lcd;

void st7789lcd_init(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT, uint8_t ROTATION, uint16_t HEIGHT, uint16_t WIDTH) // TFT(ST7789) LCD display initialisation
{
    st7789lcd.CS = CS;
    st7789lcd.CS_PORT = CS_PORT;
    st7789lcd.DC = DC;
    st7789lcd.DC_PORT = DC_PORT;
    st7789lcd.RST = RST;
    st7789lcd.RST_PORT = RST_PORT;
    st7789lcd.LED = LED;
    st7789lcd.LED_PORT = LED_PORT;

    st7789lcd.ROTATION = ROTATION;

    st7789lcd.HEIGHT = HEIGHT;
    st7789lcd.WIDTH = WIDTH;

    gpio_en(DC_PORT);  // Enable DC pin port
    gpio_en(RST_PORT); // Enable RST pin port

    gpio_setup(DC, DC_PORT, 3, 0);   // Set as output at 2MHz push pull mode
    gpio_setup(RST, RST_PORT, 3, 0); // Set as output at 2MHz push pull mode

    spi1_slaveset(CS, CS_PORT, 3); // Set CS pin

    if (!(LED_PORT == 'V' || LED_PORT == 'v')) // If the LED pin is not VCC
    {
        gpio_en(LED_PORT);               // Enable LED pin port
        gpio_setup(LED, LED_PORT, 3, 0); // Set as output at 2MHz push pull mode
        gpio_setreset(LED, LED_PORT, 1); // Set LED pin
    }

    gpio_setreset(RST, RST_PORT, 0);        // Reset RST pin
    for (volatile int i = 0; i <= 160; i++) // Delay of ~50mS
        for (volatile int j = 0; j <= 160; j++)
            ;
    gpio_setreset(RST, RST_PORT, 1); // Set RST pin

    spi1_slaveselect(CS, CS_PORT, 1); // Select slave

    // Software Reset
    gpio_setreset(DC, DC_PORT, 0);           // Reset DC pin for cmd
    spi1_8w1byte(0x01);                      // Software Reset cmd
    for (volatile int i = 0; i <= 1100; i++) // Delay of ~150mS
        for (volatile int j = 0; j <= 1100; j++)
            ;

    // Sleep Out
    spi1_8w1byte(0x11);                      // Sleep Out cmd
    for (volatile int i = 0; i <= 1100; i++) // Delay of ~150mS
        for (volatile int j = 0; j <= 1100; j++)
            ;

    st7789lcd_setup(); // Sets display orientation and RGB color path

    // Porch Setting
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xB2);            // Porch Setting cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x0C);            // Data Parameter
    spi1_8w1byte(0x0C);            // Data Parameter
    spi1_8w1byte(0x00);            // Data Parameter
    spi1_8w1byte(0x33);            // Data Parameter
    spi1_8w1byte(0x33);            // Data Parameter

    // Gate Control
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xB7);            // Gate Control cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x35);            // Data Parameter

    // VCOM Setting
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xBB);            // VCOM Setting cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x19);            // Data Parameter

    // VRH Set
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xC3);            // VRH Set cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x12);            // Data Parameter

    // VDV Set
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xC4);            // VDV Set cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x20);            // Data Parameter

    // Frame Rate Control in Normal Mode
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xC6);            // Frame Rate Control in Normal Mode cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x0F);            // Data Parameter(60Hz refresh rate)

    // Power Control 1
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xD0);            // Power Control 1 cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0xA4);            // Data Parameter
    spi1_8w1byte(0xA1);            // Data Parameter

    // Display Inversion OFF
    gpio_setreset(DC, DC_PORT, 0);         // Reset DC pin for cmd
    spi1_8w1byte(0x20);                    // Display Inversion OFF cmd
    for (volatile int i = 0; i <= 80; i++) // Delay of ~10mS
        for (volatile int j = 0; j <= 80; j++)
            ;

    // Normal Display Mode ON
    spi1_8w1byte(0x13);                    // Normal Display Mode ON cmd
    for (volatile int i = 0; i <= 80; i++) // Delay of ~10mS
        for (volatile int j = 0; j <= 80; j++)
            ;

    // Display ON
    spi1_8w1byte(0x29);                     // Display ON cmd
    for (volatile int i = 0; i <= 160; i++) // Delay of ~50mS
        for (volatile int j = 0; j <= 160; j++)
            ;

    spi1_slaveselect(CS, CS_PORT, 0); // De-select slave device
}

void st7789lcd_setup() // Sets display orientation and RGB settings
{
    uint8_t MADTCL_data = 0;
    switch (st7789lcd.ROTATION)
    {
    case 0:
        MADTCL_data = 0x00;
        break;
    case 1:
        MADTCL_data = 0x60;
        break;
    case 2:
        MADTCL_data = 0xC0;
        break;
    case 3:
        MADTCL_data = 0xA0;
        break;
    default:
        MADTCL_data = 0x00;
        break;
    }

    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 0); // Reset DC pin for cmd

    // Memory Data Access Control(MDACTL)
    /*
     * Command Opcode: 0x36 (0011 0110)
     * Description:   MADCTL (Memory Access Control Command)
     * Function:      Signals the display controller to accept memory access / orientation setup data on the next byte.
     */
    spi1_8w1byte(0x36);                                // Memory Data Access Control cmd
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 1); // Set DC pin for data
    /*
     * MADCTL Parameter Data Byte (Configured Bits)
     * Bit 7 (MY)  - Page Address Order    : 0 = Top to Bottom, 1 = Bottom to Top
     * Bit 6 (MX)  - Column Address Order  : 0 = Left to Right, 1 = Right to Left
     * Bit 5 (MV)  - Page/Column Exchange  : 0 = Normal (Portrait), 1 = Reverse (Landscape)
     * Bit 4 (ML)  - Vertical Refresh      : 0 = Top to Bottom, 1 = Bottom to Top
     * Bit 3 (RGB) - Color Panel Select    : 0 = RGB Order, 1 = BGR Order
     * Bit 2 (MH)  - Horizontal Refresh    : 0 = Left to Right, 1 = Right to Left
     * Bit 1 (RSV) - Reserved Bit          : Keep 0
     * Bit 0 (RSV) - Reserved Bit          : Keep 0
     */
    spi1_8w1byte(MADTCL_data); // Sets RGB color path and orientation bits

    // Interface Pixel Format
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x3A);                                // Interface Pixel Format cmd
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x55);                                // 16-bit RGB565 color mode
    for (volatile int i = 0; i <= 80; i++)             // Delay of ~10mS
        for (volatile int j = 0; j <= 80; j++)
            ;
}

void st7789lcd_setsize(uint8_t FULL_SCREEN, uint16_t ROW_START, uint16_t COL_START, uint16_t ROW_END, uint16_t COL_END) // Sets pixel grid size
{
    uint16_t row_start = ROW_START;
    uint16_t row_end = ROW_END - 1;
    uint16_t col_start = COL_START;
    uint16_t col_end = COL_END - 1;

    if ((FULL_SCREEN == 1) && (st7789lcd.ROTATION == 1 || st7789lcd.ROTATION == 3))
    {
        row_start = COL_START;
        row_end = COL_END - 1;
        col_start = ROW_START;
        col_end = ROW_END - 1;
    }

    // Row Address Set
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x2B);                                // Row Address Set cmd
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 1); // Set DC pin for data
    // Start Row:
    spi1_8w1byte((uint8_t)(row_start >> 8));
    spi1_8w1byte((uint8_t)row_start);
    // End Row:
    spi1_8w1byte((uint8_t)(row_end >> 8));
    spi1_8w1byte((uint8_t)row_end);

    // Column Address Set
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x2A);                                // Column Address Set cmd
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 1); // Set DC pin for data
    // Start Column:
    spi1_8w1byte((uint8_t)(col_start >> 8));
    spi1_8w1byte((uint8_t)col_start);
    // End Column:
    spi1_8w1byte((uint8_t)(col_end >> 8));
    spi1_8w1byte((uint8_t)col_end);
}

void st7789lcd_clear(uint16_t BG_COLOR) // Display clear
{
    st7789lcd.BG_COLOR = BG_COLOR; // Save back ground color

    spi1_slaveselect(st7789lcd.CS, st7789lcd.CS_PORT, 1); // Select slave

    st7789lcd_setsize(1, 0, 0, st7789lcd.HEIGHT, st7789lcd.WIDTH);

    // Memory Write
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 0);                      // Reset DC pin for cmd
    spi1_8w1byte(0x2C);                                                     // Memory Write cmd
    gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 1);                      // Set DC pin for data
    for (volatile int i = 0; i < (st7789lcd.HEIGHT * st7789lcd.WIDTH); i++) // Set display to the required RGB color
    {
        spi1_8wf1byte((uint8_t)(BG_COLOR >> 8));
        spi1_8wf1byte((uint8_t)BG_COLOR);
    }

    spi1_slaveselect(st7789lcd.CS, st7789lcd.CS_PORT, 0); // De-select slave device
}

void st7789lcd_setbox(uint8_t BOX_NUM, uint16_t RS, uint16_t RE, uint16_t CS, uint16_t CE) // Saves box info in the text box array
{
    st7789lcd.TEXT_BOX[BOX_NUM * 4] = RS;       // Row start
    st7789lcd.TEXT_BOX[(BOX_NUM * 4) + 1] = RE; // Row end
    st7789lcd.TEXT_BOX[(BOX_NUM * 4) + 2] = CS; // Col start
    st7789lcd.TEXT_BOX[(BOX_NUM * 4) + 3] = CE; // Col end
}

void st7789lcd_inbox(uint8_t BOX_NUM)
{
    st7789lcd.BOX_NUM = BOX_NUM;
}

void st7789lcd_rowrst() // Resets row addr to 0
{
    st7789lcd.ROW_ADDR[st7789lcd.BOX_NUM] = 0;
}

void st7789lcd_colrst() // Resets col addr to 0
{
    st7789lcd.COL_ADDR[st7789lcd.BOX_NUM] = 0;
}

void st7789lcd_addrst() // Resets row and col addr to 0
{
    st7789lcd.ROW_ADDR[st7789lcd.BOX_NUM] = 0;
    st7789lcd.COL_ADDR[st7789lcd.BOX_NUM] = 0;
}

void st7789lcd_alladdrst() // Resets all row and col addr to 0
{
    for (volatile int i = 0; i < 4; i++)
    {
        st7789lcd.ROW_ADDR[i] = 0;
        st7789lcd.COL_ADDR[i] = 0;
    }
}

void st7789lcd_settext(uint8_t TEXT_ROW_PIXEL_COUNT, uint8_t TEXT_COL_PIXEL_COUNT) // Sets font pixel count
{
    st7789lcd.TEXT_ROW_PIXEL_COUNT = TEXT_ROW_PIXEL_COUNT;
    st7789lcd.TEXT_COL_PIXEL_COUNT = TEXT_COL_PIXEL_COUNT;
}

void st7789lcd_print(uint8_t *TEXT, uint16_t ROW_OFFSET, uint16_t COL_OFFSET, uint16_t TEXT_COLOR, uint8_t TEXT_SIZE) // Display write function
{
    uint16_t BOX_RS = st7789lcd.TEXT_BOX[st7789lcd.BOX_NUM * 4];       // Row start
    uint16_t BOX_RE = st7789lcd.TEXT_BOX[(st7789lcd.BOX_NUM * 4) + 1]; // Row end
    uint16_t BOX_CS = st7789lcd.TEXT_BOX[(st7789lcd.BOX_NUM * 4) + 2]; // Col start
    uint16_t BOX_CE = st7789lcd.TEXT_BOX[(st7789lcd.BOX_NUM * 4) + 3]; // Col end

    uint16_t temp_prev_row_addr = st7789lcd.ROW_ADDR[st7789lcd.BOX_NUM]; // Last used row address
    uint16_t temp_prev_col_addr = st7789lcd.COL_ADDR[st7789lcd.BOX_NUM]; // Last used col address

    // Pixel location calculation
    uint16_t temp_row_start_addr = BOX_RS + temp_prev_row_addr + ROW_OFFSET;                         // Row start address
    uint16_t temp_row_end_addr = temp_row_start_addr + (st7789lcd.TEXT_ROW_PIXEL_COUNT * TEXT_SIZE); // Row end address
    uint16_t temp_col_start_addr = BOX_CS + temp_prev_col_addr + COL_OFFSET;                         // Column start address
    uint16_t temp_col_end_addr = temp_col_start_addr + (st7789lcd.TEXT_COL_PIXEL_COUNT * TEXT_SIZE); // Column end address

    spi1_slaveselect(st7789lcd.CS, st7789lcd.CS_PORT, 1); // Select slave

    uint8_t count = 0;          // Charecter counter
    uint8_t col_count = 0;      // Number of charecters present in column
    while (TEXT[count] != '\0') // Charecter check
    {
        // Next char addr based on the previous one
        temp_row_end_addr = temp_row_start_addr + (st7789lcd.TEXT_ROW_PIXEL_COUNT * TEXT_SIZE);
        temp_col_start_addr = BOX_CS + temp_prev_col_addr + COL_OFFSET + (st7789lcd.TEXT_COL_PIXEL_COUNT * col_count * TEXT_SIZE);
        temp_col_end_addr = temp_col_start_addr + (st7789lcd.TEXT_COL_PIXEL_COUNT * TEXT_SIZE);

        if (temp_row_end_addr > BOX_RE || temp_col_end_addr > BOX_CE) // If row or col borders are crossed
        {
            if (count == 0) // Break if no space for even a single char is available
                break;

            if (temp_col_end_addr > BOX_CE) // Box col end border checker
            {
                temp_row_start_addr = temp_row_end_addr;                                                // Shift to the next row slot
                temp_row_end_addr = temp_row_start_addr + (st7789lcd.TEXT_ROW_PIXEL_COUNT * TEXT_SIZE); // End of next row slot
                temp_col_start_addr = BOX_CS;                                                           // Reset to the initial col addr
            }

            if (temp_row_end_addr > BOX_RE) // Box row end border checker
            {
                temp_row_start_addr = BOX_RS + temp_prev_row_addr + ROW_OFFSET; // Reset to the initial row addr
                temp_col_start_addr = BOX_CS + temp_prev_col_addr + COL_OFFSET; // Reset to the initial col addr
            }

            // Update the end addr for the new start addr
            temp_row_end_addr = temp_row_start_addr + (st7789lcd.TEXT_ROW_PIXEL_COUNT * TEXT_SIZE);
            temp_col_end_addr = temp_col_start_addr + (st7789lcd.TEXT_COL_PIXEL_COUNT * TEXT_SIZE);
            col_count = 0; // Reset char count per column
        }

        st7789lcd_setsize(0, temp_row_start_addr, temp_col_start_addr, temp_row_end_addr, temp_col_end_addr); // Sets pixel box size

        uint16_t *bitmap = bitmap_char(TEXT[count]); // Charecter bitmap data

        // Memory Write
        gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 0); // Reset DC pin for cmd
        spi1_8w1byte(0x2C);                                // Memory Write cmd
        gpio_setreset(st7789lcd.DC, st7789lcd.DC_PORT, 1); // Set DC pin for data

        // Print charecter
        for (volatile int row = 0; row < st7789lcd.TEXT_ROW_PIXEL_COUNT; row++) // Row loop
        {
            int col_size = TEXT_SIZE;
            while (col_size > 0) // Text size scaling loop for rows
            {
                for (volatile int col = 0; col < st7789lcd.TEXT_COL_PIXEL_COUNT; col++) // Column loop
                {
                    int row_size = TEXT_SIZE;
                    while (row_size > 0) // Text size scaling loop for columns
                    {
                        if (bitmap[row] & (1 << ((st7789lcd.TEXT_COL_PIXEL_COUNT - 1) - col))) // For valid pixels
                        {
                            // 16 bit pixel value for charecter
                            spi1_8wf1byte((uint8_t)(TEXT_COLOR >> 8));
                            spi1_8wf1byte((uint8_t)TEXT_COLOR);
                        }
                        else // For invalid pixels
                        {
                            // 16 bit pixel value for background
                            spi1_8wf1byte((uint8_t)(st7789lcd.BG_COLOR >> 8));
                            spi1_8wf1byte((uint8_t)st7789lcd.BG_COLOR);
                        }

                        row_size--;
                    }
                }

                col_size--;
            }
        }

        count++;     // Charecter location incrimenting
        col_count++; // Char count per col incrimenting

        // Update last addr of row and col
        if (TEXT[count] == '\0')
        {
            st7789lcd.ROW_ADDR[st7789lcd.BOX_NUM] = temp_row_end_addr - BOX_RS;
            st7789lcd.COL_ADDR[st7789lcd.BOX_NUM] = temp_col_end_addr - BOX_CS;
        }
    }

    spi1_slaveselect(st7789lcd.CS, st7789lcd.CS_PORT, 0); // De-select slave device
}