#include <stdint.h>

#include "I2C_MS/I2C1.h"
#include "SPI_MS/SPI1.h"
#include "AHT10_MS/AHT10.h"
#include "BMP280_MS/BMP280.h"
#include "W25Q32_MS/W25Q32.h"
#include "ASCII_MS/ASCII.h"
#include "ST7789LCD_MS/ST7789LCD.h"
#include "MS_COLOR/COLOR.h"

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
#define DISPLAY_ORIEN 1  // TFT display orientation/rotation

int main(void)
{
    i2c1_init();
    spi1_init(3, 0);

    st7789lcd_init(TFT_CS, TFT_CS_PORT, TFT_DC, TFT_DC_PORT, TFT_RST, TFT_RST_PORT, TFT_LED, TFT_LED_PORT, DISPLAY_ORIEN, 320, 240);
    st7789lcd_clear(MS_WHITE_16);
    st7789lcd_settext(16, 12);
    st7789lcd_addrst();
    st7789lcd_print("EDGE ATMOS", 10, 40, MS_BRIGHT_RED_16, 2);

    spi1_slaveset(MEM_CS, MEM_CS_PORT, 2);

    aht10_init();

    uint8_t FACTORY_CALIBRATION_DATA[24];
    bmp280_init(FACTORY_CALIBRATION_DATA);

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

        st7789lcd_addrst();
        st7789lcd_print("TEMPERATURE :", 100, 10, MS_BLACK_16, 1);
        st7789lcd_colrst();
        st7789lcd_print(bmp_temp_char_arr, -16, 200, MS_YELLOW_16, 1);
        st7789lcd_print("'", -16, 5, MS_BLACK_16, 1);

        st7789lcd_colrst();
        st7789lcd_print("PRESURE :", 10, 10, MS_BLACK_16, 1);
        st7789lcd_colrst();
        st7789lcd_print(pres_char_arr, -16, 200, MS_GREEN_16, 1);
        st7789lcd_print("hpa", -16, 5, MS_BLACK_16, 1);

        st7789lcd_colrst();
        st7789lcd_print("HUMIDITY :", 10, 10, MS_BLACK_16, 1);
        st7789lcd_colrst();
        st7789lcd_print(humi_char_arr, -16, 200, MS_BLUE_16, 1);
        st7789lcd_print("%", -16, 5, MS_BLACK_16, 1);
    }
}