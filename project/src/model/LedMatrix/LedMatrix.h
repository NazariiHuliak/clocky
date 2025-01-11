#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <Arduino.h>
#include <FastLED.h>

#include <../src/model/BlockMatrix/BlockMatrix.h>
#include "../src/resources/font/digits.h"
#include <../src/model/Time/TimeUtils.h>

class LedMatrix {
    private:
        CFastLED FastLED;
        CRGB* leds;
        BlockMatrix blockMatrix;
        void setDigit(Position2D position2D, uint8_t digit);
    public:
        LedMatrix(CFastLED FastLED, CRGB* leds, uint16_t width, uint16_t height, uint16_t blockSize);
        ~LedMatrix();
        CRGB* getLeds();
        void setDateTimeWatchFace(DateTime currentTime, boolean mode);
};

#endif