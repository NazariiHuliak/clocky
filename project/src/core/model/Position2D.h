#ifndef POSITION2D_H
#define POSITION2D_H

#include <Arduino.h>

struct Position2D {
    uint16_t x;
    uint16_t y;

    Position2D(uint16_t xVal = 0, uint16_t yVal = 0) : x(xVal), y(yVal) {}

    void print() const {
        Serial.println("(" + String(x) + ", " + String(y) + ")");
    }
};

#endif
