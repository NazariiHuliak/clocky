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

CRGB* LedMatrix::getLeds() {
    return leds; 
}

void LedMatrix::setDateTimeWatchFace(boolean mode) {
    uint8_t* time = mode ? timeParser.getCurrentTime() : timeParser.getCurrentDate();

    this->setDigit(Position2D(7, 1), time[0] / 10);
    this->setDigit(Position2D(11, 1), time[0] % 10);
    this->setDigit(Position2D(17, 1), time[1] / 10);
    this->setDigit(Position2D(21, 1), time[1] % 10);

    setColon(true);
    setIcon();
}

void LedMatrix::setDigit(Position2D position2D, uint8_t digit) {
    for (int row = 0; row < 5; row++) {
        byte rowData = numbers[digit][row];

        int pixelRow = (position2D.getY() + row) * 24;
        for (int bitPos = 6; bitPos >= 4; bitPos--) {
            int pixelCol = position2D.getX() + (6 - bitPos);
            int pixelAbsolutPosition = pixelRow + pixelCol; 
           
            if (bitRead(rowData, bitPos)) {
                leds[blockMatrix.getMatrixIndex(pixelAbsolutPosition)] = digitColor;
            } else {
                leds[blockMatrix.getMatrixIndex(pixelAbsolutPosition)] = CHSV(0, 0, 0);
            }
        }
    }
}

void LedMatrix::setColon(boolean turnOn) {
    if (turnOn) {
        setPixel(Position2D(15, 2));
        setPixel(Position2D(15, 4));
    } else {
        setPixel(Position2D(15, 2), CHSV(0, 0, 0));
        setPixel(Position2D(15, 4), CHSV(0, 0, 0));
    }
}

void LedMatrix::setPixel(Position2D position2D) {
    setPixel(position2D, digitColor);
}

void LedMatrix::setPixel(Position2D position2D, CHSV color) {
    uint16_t pixelAbsolutPosition = position2D.getY() * blockMatrix.getWidth() + position2D.getX();
    leds[blockMatrix.getMatrixIndex(pixelAbsolutPosition)] = color;
}

void showUint8List(const uint8_t* list, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Serial.print(list[i]);
        if (i < size - 1) {
            Serial.print(", ");
        }
    }
    Serial.println();
}

void LedMatrix::setIcon() {
    for (uint8_t row = 0; row < iconHeight; row++) {
        const uint8_t* rowData = sun[row];
        
        int pixelRow = row * blockMatrix.getWidth();
        for (int col = 0; col < iconWidth; col++) {  
            int pixelAbsolutPosition = pixelRow + col; 
            int colorIndex = rowData[col];
            
            const uint8_t* color = colors[colorIndex]; 
            leds[blockMatrix.getMatrixIndex(pixelAbsolutPosition)] = CRGB(color[0], color[1], color[2]);
        }
    }
}

