#include <stdint.h>

#include "I2C_MS/I2C1.h"
#include "SPI_MS/SPI1.h"
#include "AHT10_MS/AHT10.h"
#include "BMP280_MS/BMP280.h"
#include "W25Q32_MS/W25Q32.h"
#include "ASCII_MS/ASCII.h"
#include "ST7789LCD_MS/ST7789LCD.h"
#include "COLOR_MS/COLOR.h"
#include "DYNAWEAT_MS/DYNAWEAT.h"

#define STM32F103xB
#include "STM32F103_CMSIS/stm32f1xx.h"

// Pins and Ports
#define MEM_CS 4         // Flash CS
#define MEM_CS_PORT 'A'  // Flash CS port
#define TFT_CS 3         // TFT display CS
#define TFT_CS_PORT 'A'  // TFT display CS port
#define TFT_DC 1         // TFT display DC
#define TFT_DC_PORT 'A'  // TFT display DC port
#define TFT_RST 2        // TFT display RST
#define TFT_RST_PORT 'A' // TFT display RST port
#define TFT_LED 0        // TFT display LED
#define TFT_LED_PORT 'A' // TFT display LED port
#define DISPLAY_ORIEN 3  // TFT display orientation/rotation

int main(void)
{
    // Initialisations of drivers
    i2c1_init();     // I2C initialisation
    spi1_init(3, 0); // SPI initialisation

    aht10_init();                          // AHT10 sensor initialisation
    uint8_t FACTORY_CALIBRATION_DATA[24];  // BMP280 factory calibration data
    bmp280_init(FACTORY_CALIBRATION_DATA); // BMP280 sensor initialisation

    st7789lcd_pinset(TFT_CS, TFT_CS_PORT, TFT_DC, TFT_DC_PORT, TFT_RST, TFT_RST_PORT, TFT_LED, TFT_LED_PORT); // Display pin configuration
    st7789lcd_dispset(DISPLAY_ORIEN, 320, 240);                                                               // Display settings
    st7789lcd_init();                                                                                         // TFT display initialisation

    // Display setup
    st7789lcd_clearall(MS_WHITE_16);
    st7789lcd_settext(16, 12);

    st7789lcd_setbox(1, 0, 60, 0, 320);
    st7789lcd_setbox(2, 60, 150, 0, 150);
    st7789lcd_setbox(3, 60, 150, 150, 250);
    st7789lcd_setbox(4, 60, 150, 250, 320);
    st7789lcd_setbox(5, 150, 240, 0, 320);

    st7789lcd_inbox(1);
    st7789lcd_fillbox(MS_GREEN_16);
    st7789lcd_print("EDGE ATMOS", 10, 40, MS_BRIGHT_RED_16, 2);

    st7789lcd_inbox(2);
    st7789lcd_fillbox(MS_YELLOW_16);
    st7789lcd_print("TEMPERATURE", 10, 10, MS_BLACK_16, 1);
    st7789lcd_colrst();
    st7789lcd_print("PRESURE", 10, 10, MS_BLACK_16, 1);
    st7789lcd_colrst();
    st7789lcd_print("HUMIDITY", 10, 10, MS_BLACK_16, 1);

    st7789lcd_inbox(4);
    st7789lcd_fillbox(MS_CYAN_16);
    st7789lcd_print("'", 10, 10, MS_BLACK_16, 1);
    st7789lcd_colrst();
    st7789lcd_print("hpa", 10, 10, MS_BLACK_16, 1);
    st7789lcd_colrst();
    st7789lcd_print("%", 10, 10, MS_BLACK_16, 1);

    st7789lcd_inbox(5);
    st7789lcd_fillbox(MS_ORANGE_16);
    st7789lcd_print("THIS IS THE 5TH WINDOW FOR SOME REASON", 10, 10, MS_BLACK_16, 1);

    st7789lcd_inbox(3);
    st7789lcd_fillbox(MS_LIGHT_GRAY_16);

    while (1)
    {
        float aht_temp = 0;
        float bmp_temp = 0;
        float humi = 0;
        float pres = 0;
        aht10_read(&aht_temp, &humi);
        bmp280_read(FACTORY_CALIBRATION_DATA, &pres, &bmp_temp);

        uint8_t aht_temp_char_arr[6];
        uint8_t bmp_temp_char_arr[6];
        uint8_t humi_char_arr[6];
        uint8_t pres_char_arr[6];
        num_float4digi_ascii(aht_temp, aht_temp_char_arr);
        num_float4digi_ascii(bmp_temp, bmp_temp_char_arr);
        num_float4digi_ascii(humi, humi_char_arr);
        num_float4digi_ascii(pres, pres_char_arr);

        uint16_t temp_color = dynaweat_temp16(bmp_temp);
        uint16_t pres_color = dynaweat_pres16(pres);
        uint16_t humi_color = dynaweat_humi16(humi);

        st7789lcd_addrst();
        st7789lcd_print(bmp_temp_char_arr, 10, 10, temp_color, 1);
        st7789lcd_colrst();
        st7789lcd_print(pres_char_arr, 10, 10, pres_color, 1);
        st7789lcd_colrst();
        st7789lcd_print(humi_char_arr, 10, 10, humi_color, 1);
    }
}
