#ifndef POSITION2D_H
#define POSITION2D_H

#include <Arduino.h>

struct Position2D {
    int16_t x;
    int16_t y;

    Position2D(int16_t xVal = 0, int16_t yVal = 0) : x(xVal), y(yVal) {}

    void print() const {
        Serial.println("(" + String(x) + ", " + String(y) + ")");
    }
};

#endif
