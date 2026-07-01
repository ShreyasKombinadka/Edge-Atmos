#include "ST7789LCD.h"
#include "../SPI_MS/SPI1.h"
#include "./GPIO_MS/GPIO.h"

#include <stdint.h>

void st7789lcd_init(uint8_t CS, uint8_t CS_PORT, uint8_t DC, uint8_t DC_PORT, uint8_t RST, uint8_t RST_PORT, uint8_t LED, uint8_t LED_PORT)
{
    gpio_en(CS_PORT);  // Enable CS pin port
    gpio_en(DC_PORT);  // Enable DC pin port
    gpio_en(RST_PORT); // Enable RST pin port
    gpio_en(LED_PORT); // Enable LED pin port

    gpio_setup(CS, CS_PORT, 2);   // Set as output at 2MHz push pull mode
    gpio_setup(DC, DC_PORT, 2);   // Set as output at 2MHz push pull mode
    gpio_setup(RST, RST_PORT, 2); // Set as output at 2MHz push pull mode
    gpio_setup(LED, LED_PORT, 2); // Set as output at 2MHz push pull mode

    if (!(LED_PORT == 'V'))              // If the LED pin is not VCC
        gpio_setreset(LED, LED_PORT, 1); // Set LED pin

    gpio_setreset(RST, RST_PORT, 0);        // Reset RST pin
    for (volatile int i = 0; i <= 160; i++) // Delay of 50mS
        for (volatile int j = 0; j <= 160; j++)
            ;
    gpio_setreset(RST, RST_PORT, 1); // Set RST pin

    spi1_slaveselect(CS, CS_PORT, 1); // Select slave

    gpio_setreset(DC, DC_PORT, 0);           // Reset DC pin for cmd
    spi1_8w1byte(0x01);                      // Software Reset cmd
    for (volatile int i = 0; i <= 1100; i++) // Delay of 150mS
        for (volatile int j = 0; j <= 1100; j++)
            ;

    spi1_8w1byte(0x11);                      // Sleep Out cmd
    for (volatile int i = 0; i <= 1100; i++) // Delay of 150mS
        for (volatile int j = 0; j <= 1100; j++)
            ;

    spi1_8w1byte(0x36);            // Memory Data Access Control cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x00);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x3A);            // Interface Pixel Format cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x55);            // Data Parameter(16-bit RGB565 color mode)

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xB2);            // Interface Pixel Format cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x0C);            // Data Parameter
    spi1_8w1byte(0x0C);            // Data Parameter
    spi1_8w1byte(0x00);            // Data Parameter
    spi1_8w1byte(0x33);            // Data Parameter
    spi1_8w1byte(0x33);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xB7);            // Gate Control cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x35);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xBB);            // VCOM Setting cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x19);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xC3);            // VRH Set cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x12);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xC4);            // VDV Set cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x20);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xC6);            // Frame Rate Control in Normal Mode cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0x0F);            // Data Parameter(60Hz refresh rate)

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0xD0);            // Frame Rate Control in Normal Mode cmd
    gpio_setreset(DC, DC_PORT, 1); // Set DC pin for data
    spi1_8w1byte(0xA4);            // Data Parameter
    spi1_8w1byte(0xA1);            // Data Parameter

    gpio_setreset(DC, DC_PORT, 0); // Reset DC pin for cmd
    spi1_8w1byte(0x21);            // Display Inversion ON cmd

    spi1_8w1byte(0x29);                     // Display ON
    for (volatile int i = 0; i <= 160; i++) // Delay of 50mS
        for (volatile int j = 0; j <= 160; j++)
            ;

    spi1_slaveselect(CS, CS_PORT, 0); // De-select slave device
}