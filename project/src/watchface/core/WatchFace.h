#ifndef WATCHFACE_H
#define WATCHFACE_H

#include <FastLED.h>

#include <../src/core/model/Position2D/Position2D.h>
#include <../src/utils/matrix/MatrixUtil.h>
#include <../src/resources/font/digits.h>
#include <../src/resources/icon/weather.h>
#include <../src/resources/icon/colors.h>

class WatchFace {
    CHSV digitColor{ 0, 0, 255 };
    CRGB* leds;

public:
    explicit WatchFace(CRGB* leds) : leds(leds) {}
    virtual ~WatchFace() = default;

    virtual unsigned long getUpdateDataPeriod() = 0;
    virtual unsigned long getLastTimeDataUpdate() = 0;

    virtual void showFrame(int16_t xOffset = 0) = 0;
    virtual void nextMode() = 0;
    virtual void resetMode() = 0;

    virtual bool isUpdateAllowed() = 0;
    virtual void updateData(unsigned long updateTime) = 0;

    void setDigit(Position2D position2D, uint8_t digit) const;
    void setColon(bool turnOn, Position2D offsetPosition) const;
    void setPixel(Position2D position2D) const;
    void setPixel(Position2D position2D, CHSV color) const;
    void setIcon(Position2D position2D, const uint8_t(&icon)[8][7], bool isMonotone) const;
};

#endif