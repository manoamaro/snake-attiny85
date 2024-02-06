#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <avr/io.h>

#define _INPUT 0
#define _OUTPUT 1
#define _INPUT_PULLUP 2
#define _HIGH 1
#define _LOW 0

namespace utils {
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t value);
    uint8_t digitalRead(uint8_t pin);
}

#endif