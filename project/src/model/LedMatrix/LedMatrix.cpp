#include <Arduino.h>
#include "LedMatrix.h"

LedMatrix::LedMatrix(CFastLED FastLED, CRGB* leds, iarduino_RTC& clock, uint16_t width, uint16_t height, uint16_t blockSize)
    : blockMatrix(width, height, blockSize), timeParser(clock) {
    this->FastLED = FastLED;
    this->leds = leds;
}

LedMatrix::~LedMatrix() {
    delete[] leds; 
}

void LedMatrix::setDateTimeWatchFace(boolean mode) {
    uint8_t* time = mode ? timeParser.getCurrentTime() : timeParser.getCurrentDate();
    
    this->setDigit(Position2D(16, 6), (time[0] / 10));
    this->setDigit(Position2D(12, 6), time[0] % 10);
    this->setDigit(Position2D(6, 6), time[1] / 10);
    this->setDigit(Position2D(2, 6), time[1] % 10);
}

CRGB* LedMatrix::getLeds() {
    return leds; 
}

void LedMatrix::setDigit(Position2D position2D, uint8_t digit) {
    for (int row = 0; row < 5; row++) {
        byte rowData = numbers[digit][row];

        for (int bitPos = 6; bitPos >= 4; bitPos--) {
            int pixelRowStart = (position2D.getY() - row) * 24;
            int pixelColStart = position2D.getX() - (6 - bitPos);
            int pixelAbsolutPosition = pixelRowStart + pixelColStart;
            if (bitRead(rowData, bitPos)) {
                leds[blockMatrix.getMatrixIndex(pixelAbsolutPosition)] = CHSV(0, 0, 255);
            } else {
                leds[blockMatrix.getMatrixIndex(pixelAbsolutPosition)] = CHSV(0, 0, 0);
            }
        }
    }
}