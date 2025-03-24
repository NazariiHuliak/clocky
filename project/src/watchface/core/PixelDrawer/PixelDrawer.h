#ifndef PIXELDRAWER_H
#define PIXELDRAWER_H

#include <FastLED.h>
#include <../src/utils/matrix/MatrixUtil.h>
#include <../src/resources/font/digits.h>
#include <../src/resources/icon/weather.h>
#include <../src/resources/icon/colors.h>
#include <../src/core/model/Position2D.h>

class PixelDrawer {
    CRGB *leds;

public:
    CHSV digitColor{0, 0, 255};
    CHSV airAlertBorderColor{0, 0, 100};

    explicit PixelDrawer(CRGB *leds) : leds(leds) {
    }

    void setDigit(Position2D position2D, uint8_t digit) const;

    void setLetter(Position2D position2D, char letter) const;

    void setSentence(Position2D position2D, const char* sentence) const;

    void setColon(bool turnOn, Position2D offsetPosition) const;

    void setPixel(Position2D position2D) const;

    void setPixel(Position2D position2D, CHSV color) const;

    void setIcon(Position2D position2D, const uint8_t (&icon)[ICON_HEIGHT][ICON_WIDTH], bool isMonotone) const;
};

#endif
