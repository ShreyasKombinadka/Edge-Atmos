#include "ST7789LCD.h"
#include "../SPI_MS/SPI1.h"
#include "./GPIO_MS/GPIO.h"

#include <stdint.h>

// TFT(ST7789) LCD display without touch initialisation
void st7789lcd_init(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT)
{
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

    // Memory Data Access Control
    spi1_8w1byte(0x36);            // Memory Data Access Control cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0xC8);            // Sets RGB color path and orientation bits

    // Interface Pixel Format
    gpio_setreset(DC, DC_PORT, 0);         // Reset DC pin for cmd
    spi1_8w1byte(0x3A);                    // Interface Pixel Format cmd
    gpio_setreset(DC, DC_PORT, 1);         // Set DC pin for data
    spi1_8w1byte(0x55);                    // 16-bit RGB565 color mode
    for (volatile int i = 0; i <= 80; i++) // Delay of ~10mS
        for (volatile int j = 0; j <= 80; j++)
            ;

    // Column Address Set
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x2A);            // Column Address Set cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    // Start Column: 0
    spi1_8w1byte(0x00);
    spi1_8w1byte(0x00);
    // End Column: 239
    spi1_8w1byte(0x00);
    spi1_8w1byte(0xEF);

    // Row Address Set
    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x2B);            // Row Address Set cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    // Start Column: 0
    spi1_8w1byte(0x00);
    spi1_8w1byte(0x00);
    // End Column: 319
    spi1_8w1byte(0x01);
    spi1_8w1byte(0x3F);

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

    // Display Inversion ON
    gpio_setreset(DC, DC_PORT, 0);         // Reset DC pin for cmd
    spi1_8w1byte(0x21);                    // Display Inversion ON cmd
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

void st7789lcd_fillcolor(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint16_t FILL_COLOR) // Fill display
{
    spi1_slaveselect(CS, CS_PORT, 1); // Select slave

    // Memory Write
    gpio_setreset(DC, DC_PORT, 0);           // Reset DC pin for cmd
    spi1_8w1byte(0x2C);                      // Memory Write cmd
    gpio_setreset(DC, DC_PORT, 1);           // Set DC pin for data
    for (volatile int i = 0; i < 76800; i++) // Color filling
    {
        spi1_8wf1byte((uint8_t)(FILL_COLOR >> 8));
        spi1_8wf1byte((uint8_t)FILL_COLOR);
    }

    spi1_slaveselect(CS, CS_PORT, 0); // De-select slave device
}

void st7789lcd_disptest(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT) // Display test
{
    /*
    spi1_slaveselect(CS, CS_PORT, 1); // Select slave

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd

    // Display Inversion ON
    spi1_8w1byte(0x21);                     // Display Inversion ON cmd
    for (volatile int i = 0; i <= 710; i++) // Delay of ~1S
        for (volatile int j = 0; j <= 710; j++)
            ;
    // Display Inversion OFF
    spi1_8w1byte(0x20);                     // Display Inversion OFF cmd
    for (volatile int i = 0; i <= 710; i++) // Delay of ~1S
        for (volatile int j = 0; j <= 710; j++)
            ;

    spi1_slaveselect(CS, CS_PORT, 0); // De-select slave device
    */

    st7789lcd_fillcolor(CS, CS_PORT, DC, DC_PORT, 0xF800);
}