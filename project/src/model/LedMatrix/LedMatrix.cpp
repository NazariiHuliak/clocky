#include <Arduino.h>
#include "LedMatrix.h"

LedMatrix::LedMatrix(CFastLED FastLED, CRGB* leds, uint16_t width, uint16_t height, uint16_t blockSize): blockMatrix(width, height, blockSize) {
    this->FastLED = FastLED;
    this->leds = leds;
}

LedMatrix::~LedMatrix() {
    delete[] leds; 
}

void LedMatrix::setDateTimeWatchFace(DateTime currentTime, boolean mode) {
    uint8_t* timeDigits = mode ? getParsedCurrentTime(currentTime) : getParsedCurrentDate(currentTime);
    
    this->setDigit(Position2D(16, 6), timeDigits[0]);
    this->setDigit(Position2D(12, 6), timeDigits[1]);
    this->setDigit(Position2D(6, 6), timeDigits[2]);
    this->setDigit(Position2D(2, 6), timeDigits[3]);

    delete[] timeDigits;
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