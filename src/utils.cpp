#include "utils.h"

void utils::pinMode(uint8_t pin, uint8_t mode) {
        switch (mode) {
        case _INPUT:
            // Set pin as input
            DDRB &= ~(1 << pin);
            // Disable pull-up resistor
            PORTB &= ~(1 << pin);
            break;
        case _OUTPUT:
            // Set pin as output
            DDRB |= (1 << pin);
            // Disable pull-up resistor
            PORTB &= ~(1 << pin);
            break;
        case _INPUT_PULLUP:
            // Set pin as input with pull-up resistor enabled
            DDRB &= ~(1 << pin);
            PORTB |= (1 << pin);
            break;
    }
}

void utils::digitalWrite(uint8_t pin, uint8_t value) {
    if (value == _HIGH) {
        PORTB |= (1 << pin); // Set pin high
    } else {
        PORTB &= ~(1 << pin); // Set pin low
    }
}

uint8_t utils::digitalRead(uint8_t pin) {
    return (PINB & (1 << pin)) ? 1 : 0;
}
