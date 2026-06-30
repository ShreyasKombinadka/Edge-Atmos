#include "GPIO.h"
#include <stdint.h>

#define STM32F103xB
#include "stm32f1xx.h"

void gpio_en(uint8_t GPIO_PORT) // GPIO PORT CLK enable
{
    switch (GPIO_PORT)
    {
    case 'A':                     // GPIOA
        RCC->APB2ENR |= (1 << 2); // Enable GPIOA PORT
        break;

    case 'B':                     // GPIOB
        RCC->APB2ENR |= (1 << 3); // Enable GPIOB PORT
        break;

    case 'C':                     // GPIOC
        RCC->APB2ENR |= (1 << 4); // Enable GPIOC PORT
        break;

    case 'D':                     // GPIOD
        RCC->APB2ENR |= (1 << 5); // Enable GPIOD PORT
        break;

    case 'E':                     // GPIOE
        RCC->APB2ENR |= (1 << 6); // Enable GPIOE PORT
        break;
    }
}

void gpio_setpin(uint8_t PIN, uint8_t PIN_PORT, uint8_t PIN_CNF_MODE) // Set PIN configuration and mode
/*(PIN_CNF_MODE & 0x0F):
(PIN_CNF_MODE & 0x0F)[0] = MODE[0]
(PIN_CNF_MODE & 0x0F)[1] = MODE[1]
(PIN_CNF_MODE & 0x0F)[2] = CNF[0]
(PIN_CNF_MODE & 0x0F)[3] = CNF[1]

MODE:
Bits 0-1: Port n.0 mode bits.
Allowed values:
0: Input: Input mode (reset state)
1: Output: Output mode 10 MHz
2: Output2: Output mode 2 MHz
3: Output50: Output mode 50 MHz

CNF:
Bits 2-3: Port n.0 configuration bits.
Allowed values:
0: PushPull: Analog mode / Push-Pull mode
1: OpenDrain: Floating input (reset state) / Open Drain-Mode
2: AltPushPull: Input with pull-up/pull-down / Alternate Function Push-Pull Mode
3: AltOpenDrain: Alternate Function Open-Drain Mode
 */
{
    switch (PIN_PORT)
    {
    case 'A': // GPIOA
        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOA->CRL &= ~(0xF << (PIN * 4));                  // Clear configuration
                GPIOA->CRL |= ((PIN_CNF_MODE & 0x0F) << (PIN * 4)); // Set PIN configuration and mode
            }
            else
            {
                GPIOA->CRH &= ~(0xF << ((PIN - 8) * 4));                  // Clear configuration
                GPIOA->CRH |= ((PIN_CNF_MODE & 0x0F) << ((PIN - 8) * 4)); // Set PIN configuration and mode
            }
        }

        break;

    case 'B': // GPIOB
        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOB->CRL &= ~(0xF << (PIN * 4));                  // Clear configuration
                GPIOB->CRL |= ((PIN_CNF_MODE & 0x0F) << (PIN * 4)); // Set PIN configuration and mode
            }
            else
            {
                GPIOB->CRH &= ~(0xF << ((PIN - 8) * 4));                  // Clear configuration
                GPIOB->CRH |= ((PIN_CNF_MODE & 0x0F) << ((PIN - 8) * 4)); // Set PIN configuration and mode
            }
        }

        break;

    case 'C': // GPIOC
        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOC->CRL &= ~(0xF << (PIN * 4));                  // Clear configuration
                GPIOC->CRL |= ((PIN_CNF_MODE & 0x0F) << (PIN * 4)); // Set PIN configuration and mode
            }
            else
            {
                GPIOC->CRH &= ~(0xF << ((PIN - 8) * 4));                  // Clear configuration
                GPIOC->CRH |= ((PIN_CNF_MODE & 0x0F) << ((PIN - 8) * 4)); // Set PIN configuration and mode
            }
        }

        break;

    case 'D': // GPIOD
        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOD->CRL &= ~(0xF << (PIN * 4));                  // Clear configuration
                GPIOD->CRL |= ((PIN_CNF_MODE & 0x0F) << (PIN * 4)); // Set PIN configuration and mode
            }
            else
            {
                GPIOD->CRH &= ~(0xF << ((PIN - 8) * 4));                  // Clear configuration
                GPIOD->CRH |= ((PIN_CNF_MODE & 0x0F) << ((PIN - 8) * 4)); // Set PIN configuration and mode
            }
        }

        break;

    case 'E': // GPIOE
        if (PIN < 16)
        {
            if (PIN < 8)
            {
                GPIOE->CRL &= ~(0xF << (PIN * 4));                  // Clear configuration
                GPIOE->CRL |= ((PIN_CNF_MODE & 0x0F) << (PIN * 4)); // Set PIN configuration and mode
            }
            else
            {
                GPIOE->CRH &= ~(0xF << ((PIN - 8) * 4));                  // Clear configuration
                GPIOE->CRH |= ((PIN_CNF_MODE & 0x0F) << ((PIN - 8) * 4)); // Set PIN configuration and mode
            }
        }

        break;
    }
}