#include "WatchFace.h"

void WatchFace::setDigit(Position2D position2D, uint8_t digit) {
    for (uint8_t row = 0; row < 5; row++) {
        byte rowData = numbers[digit][row];

        uint16_t pixelRow = position2D.getY() + row;
        if (pixelRow < 0 || pixelRow > height - 1) continue;
        
        uint16_t pixelNumberInRows = pixelRow * width;
        for (uint8_t bitPos = 6; bitPos >= 4; bitPos--) {
            uint16_t pixelCol = position2D.getX() + (6 - bitPos);
            if (pixelCol < 0 || pixelCol > 23) continue;

            uint16_t pixelAbsolutPosition = pixelNumberInRows + pixelCol; 
            if (bitRead(rowData, bitPos)) {
                leds[getMatrixIndex(pixelAbsolutPosition)] = digitColor;
            } else {
                leds[getMatrixIndex(pixelAbsolutPosition)] = CHSV(0, 0, 0);
            }
        }
    }
}

void WatchFace::setColon(bool turnOn, int xOffset = 0, int yOffset = 0) {
    if (turnOn) {
        setPixel(Position2D(15 + xOffset, 2 + yOffset));
        setPixel(Position2D(15 + xOffset, 4 + yOffset));
    } else {
        setPixel(Position2D(15 + xOffset, 2 + yOffset), CHSV(0, 0, 0));
        setPixel(Position2D(15 + xOffset, 4 + yOffset), CHSV(0, 0, 0));
    }
}

void WatchFace::setPixel(Position2D position2D) {
    setPixel(position2D, digitColor);
}

void WatchFace::setPixel(Position2D position2D, CHSV color) {
    if (position2D.getX() < 0 || position2D.getX() > width - 1 || position2D.getY() < 0 || position2D.getY() > height - 1) return;

    uint16_t pixelAbsolutPosition = position2D.getY() * width + position2D.getX();
    leds[getMatrixIndex(pixelAbsolutPosition)] = color;
}

void  WatchFace::setIcon(Position2D position2D, const uint8_t (&icon)[8][7]) {
    for (uint8_t row = 0; row < iconHeight; row++) {
        const uint8_t* rowData = icon[row];
        
        uint16_t pixelRow = position2D.getY() + row;
        if (pixelRow < 0 || pixelRow > height - 1) continue;

        uint16_t pixelNumberInRows = pixelRow * width;
        for (uint8_t col = 0; col < iconWidth; col++) {  
            uint16_t pixelCol = position2D.getX() + col;
            if (pixelCol < 0 || pixelCol > width - 1) continue;

            uint16_t pixelAbsolutPosition = pixelNumberInRows + pixelCol; 
            uint16_t colorIndex = rowData[col];
            
            const uint8_t* color = colors[colorIndex]; 
            leds[getMatrixIndex(pixelAbsolutPosition)] = CRGB(color[0], color[1], color[2]);
        }
    }
}