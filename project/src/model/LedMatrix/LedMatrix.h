#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <Arduino.h>
#include <FastLED.h>
#include <iarduino_RTC.h>

#include <../src/model/BlockMatrix/BlockMatrix.h>
#include "../src/resources/font/digits.h"
#include <../src/model/Time/TimeUtils.h>


class LedMatrix {
    private:
        CFastLED FastLED;
        CRGB* leds;
        BlockMatrix blockMatrix;  
        TimeUtils timeParser;      
        void setDigit(Position2D position2D, uint8_t digit);
    public:
        LedMatrix(CFastLED FastLED, CRGB* leds, iarduino_RTC& clock, uint16_t width, uint16_t height, uint16_t blockSize); //TODO: make it with builder pattern
        ~LedMatrix();
        CRGB* getLeds();
        void setDateTimeWatchFace(boolean mode);
};

#endif