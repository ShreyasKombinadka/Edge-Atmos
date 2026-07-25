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

/*
PIN configuration and mode

(PIN_CNF_MODE & 0x0F):
(PIN_CNF_MODE & 0x0F)[3] = CNF[1]
(PIN_CNF_MODE & 0x0F)[2] = CNF[0]
(PIN_CNF_MODE & 0x0F)[1] = MODE[1]
(PIN_CNF_MODE & 0x0F)[0] = MODE[0]

MODE:
Bits 0-1: Port n.0 mode bits.
0: Input: Input mode (reset state)
1: Output: Output mode 10 MHz
2: Output2: Output mode 2 MHz
3: Output50: Output mode 50 MHz

CNF:
Bits 2-3: Port n.0 configuration bits.
0: PushPull: Analog mode / Push-Pull mode
1: OpenDrain: Floating input (reset state) / Open Drain-Mode
2: AltPushPull: Input with pull-up/pull-down / Alternate Function Push-Pull Mode
3: AltOpenDrain: Alternate Function Open-Drain Mode

Allowed values for PIN_CNF_MODE :

Input Mode (MODE = 00):
0: Input_Analog              (CNF=00, MODE=00) - 0b0000
4: Input_Floating            (CNF=01, MODE=00) - 0b0100 (Reset state)
8: Input_PullDown_PullUp     (CNF=10, MODE=00) - 0b1000
12: Reserved                 (CNF=11, MODE=00) - 0b1100

Output Mode 10 MHz (MODE = 01):
1: Output_10MHz_PushPull     (CNF=00, MODE=01) - 0b0001
5: Output_10MHz_OpenDrain    (CNF=01, MODE=01) - 0b0101
9: AltFunc_10MHz_PushPull    (CNF=10, MODE=01) - 0b1001
13: AltFunc_10MHz_OpenDrain  (CNF=11, MODE=01) - 0b1101

Output Mode 2 MHz (MODE = 10):
2: Output_2MHz_PushPull      (CNF=00, MODE=10) - 0b0010
6: Output_2MHz_OpenDrain     (CNF=01, MODE=10) - 0b0110
10: AltFunc_2MHz_PushPull    (CNF=10, MODE=10) - 0b1010
14: AltFunc_2MHz_OpenDrain   (CNF=11, MODE=10) - 0b1110

Output Mode 50 MHz (MODE = 11):
3: Output_50MHz_PushPull     (CNF=00, MODE=11) - 0b0011
7: Output_50MHz_OpenDrain    (CNF=01, MODE=11) - 0b0111
11: AltFunc_50MHz_PushPull   (CNF=10, MODE=11) - 0b1011
15: AltFunc_50MHz_OpenDrain  (CNF=11, MODE=11) - 0b1111
 */
void gpio_setup(uint8_t PIN, uint8_t PIN_PORT, uint8_t PIN_CNF_MODE) // Set PIN configuration and mode
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

void gpio_setreset(uint8_t PIN, uint8_t PIN_PORT, uint8_t SELECT) // PIN Set(SELECT = 1) or Reset(SELECT = 0)
{
    switch (PIN_PORT)
    {
    case 'A': // GPIOA
        if (PIN < 16)
        {
            if (SELECT)
                GPIOA->BSRR = (1 << PIN); // Set PIN
            else
                GPIOA->BRR = (1 << PIN); // Reset PIN
        }

        break;

    case 'B': // GPIOB
        if (PIN < 16)
        {
            if (SELECT)
                GPIOB->BSRR = (1 << PIN); // Set PIN
            else
                GPIOB->BRR = (1 << PIN); // Reset PIN
        }

        break;

    case 'C': // GPIOC
        if (PIN < 16)
        {
            if (SELECT)
                GPIOC->BSRR = (1 << PIN); // Set PIN
            else
                GPIOC->BRR = (1 << PIN); // Reset PIN
        }

        break;

    case 'D': // GPIOD
        if (PIN < 16)
        {
            if (SELECT)
                GPIOD->BSRR = (1 << PIN); // Set PIN
            else
                GPIOD->BRR = (1 << PIN); // Reset PIN
        }

        break;

    case 'E': // GPIOE
        if (PIN < 16)
        {
            if (SELECT)
                GPIOE->BSRR = (1 << PIN); // Set PIN
            else
                GPIOE->BRR = (1 << PIN); // Reset PIN
        }

        break;
    }
}
