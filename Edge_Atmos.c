#include <stdint.h>
#include "./I2C_MS/I2C1.h"
#include "./SPI_MS/SPI1.h"
#include "./ASCII_MS/ASCII.h"
#include "./LCD1602_MS/LCD1602.h"
#include "./AHT10_MS/AHT10.h"
#include "./BMP280_MS/BMP280.h"
#include "./W25Q32_MS/W25Q32.h"
#include "./ST7789LCD_MS/ST7789LCD.h"

#define STM32F103xB
#include "./STM32F103_CMSIS/stm32f1xx.h"

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

int main(void)
{
    i2c1_init();
    spi1_init(3, 0);
    lcd1602_init();

    spi1_slaveset(MEM_CS, MEM_CS_PORT);

    lcd1602_clear();
    lcd1602_print("Edge Atmos");
    for (volatile int i = 0; i < 1000000; i++)
        ;

    aht10_init();

    uint8_t FACTORY_CALIBRATION_DATA[24];
    bmp280_init(FACTORY_CALIBRATION_DATA);

    st7789lcd_init(TFT_CS, TFT_CS_PORT, TFT_DC, TFT_DC_PORT, TFT_RST, TFT_RST_PORT, TFT_LED, TFT_LED_PORT);
    lcd1602_debug("TFT init completed..!");

    st7789lcd_disptest(TFT_CS, TFT_CS_PORT, TFT_DC, TFT_DC_PORT);
    lcd1602_debug("Inversion on and off completed..!");

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

        lcd1602_clear();
        lcd1602_print("Temp: ");
        lcd1602_print(aht_temp_char_arr);
        lcd1602_char(0xDF);
        lcd1602_char('C');

        lcd1602_row2();
        lcd1602_print("Humi: ");
        lcd1602_print(humi_char_arr);
        lcd1602_char('%');

        for (volatile int i = 0; i < 1000000; i++)
            ;

        lcd1602_clear();
        lcd1602_print("Pres: ");
        lcd1602_print(pres_char_arr);
        lcd1602_print("hPa");

        lcd1602_row2();
        lcd1602_print("Temp: ");
        lcd1602_print(bmp_temp_char_arr);
        lcd1602_char(0xDF);
        lcd1602_char('C');
        for (volatile int i = 0; i < 1000000; i++)
            ;

        // Flash read for SPI functionality test
        uint8_t arr[17];
        arr[16] = '\0';
        w25q32_read(MEM_CS, MEM_CS_PORT, 0, arr, 16);
        lcd1602_debug(arr);
    }
}
