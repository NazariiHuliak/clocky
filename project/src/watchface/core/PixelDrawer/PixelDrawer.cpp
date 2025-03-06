#include "PixelDrawer.h"

void PixelDrawer::setDigit(Position2D position2D, uint8_t digit) const {
    for (uint8_t row = 0; row < 5; row++) {
        byte rowData = numbers[digit][row];

        uint16_t pixelRow = position2D.y + row;
        if (pixelRow < 0 || pixelRow > height - 1) continue;

        uint16_t pixelNumberInRows = pixelRow * width;
        for (uint8_t bitPos = 6; bitPos >= 4; bitPos--) {
            uint16_t pixelCol = position2D.x + (6 - bitPos);
            if (pixelCol < 0 || pixelCol > 23) continue;

            uint16_t pixelAbsolutPosition = pixelNumberInRows + pixelCol;

            leds[getMatrixAbsolutePosition(pixelAbsolutPosition)] = bitRead(rowData, bitPos) ? digitColor : CHSV(0, 0, 0);
        }
    }
}

void PixelDrawer::setColon(bool turnOn, Position2D offsetPosition) const {
    if (turnOn) {
        setPixel(Position2D(15 + offsetPosition.x, 2 + offsetPosition.y));
        setPixel(Position2D(15 + offsetPosition.x, 4 + offsetPosition.y));
    } else {
        setPixel(Position2D(15 + offsetPosition.x, 2 + offsetPosition.y), CHSV(0, 0, 0));
        setPixel(Position2D(15 + offsetPosition.x, 4 + offsetPosition.y), CHSV(0, 0, 0));
    }
}

void PixelDrawer::setPixel(Position2D position2D) const {
    setPixel(position2D, digitColor);
}

void PixelDrawer::setPixel(Position2D position2D, CHSV color) const {
    if (position2D.x < 0 || position2D.x > width - 1 || position2D.y < 0 || position2D.y > height - 1) return;

    uint16_t pixelAbsolutPosition = position2D.y * width + position2D.x;
    leds[getMatrixAbsolutePosition(pixelAbsolutPosition)] = color;
}

void  PixelDrawer::setIcon(Position2D position2D, const uint8_t(&icon)[ICON_HEIGHT][ICON_WIDTH], bool isMonotone) const {
    for (uint8_t row = 0; row < ICON_HEIGHT; row++) {
        const uint8_t* rowData = icon[row];

        uint16_t pixelRow = position2D.y + row;
        if (pixelRow < 0 || pixelRow > height - 1) continue;

        uint16_t pixelNumberInRows = pixelRow * width;
        for (uint8_t col = 0; col < ICON_WIDTH; col++) {
            uint16_t pixelCol = position2D.x + col;
            if (pixelCol < 0 || pixelCol > width - 1) continue;

            uint16_t pixelAbsolutPosition = pixelNumberInRows + pixelCol;
            uint16_t matrixAbsolutePosition = getMatrixAbsolutePosition(pixelAbsolutPosition);

            uint16_t colorIndex = rowData[col];

            if (isMonotone) {
                leds[matrixAbsolutePosition] = colorIndex != 0 ? digitColor : CRGB(0, 0, 0);
            } else {
                const uint8_t* color = colors[colorIndex];
                leds[getMatrixAbsolutePosition(pixelAbsolutPosition)] = CRGB(color[0], color[1], color[2]);
            }
        }
    }
}