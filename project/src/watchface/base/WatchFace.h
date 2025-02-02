#ifndef WATCHFACE_H
#define WATCHFACE_H

#include <FastLED.h>

#include <../src/model/Position2D/Position2D.h>
#include <../src/utils/matrix/MatrixUtil.h>

#include <../src/resources/font/digits.h>
#include <../src/resources/icon/weather.h>
#include <../src/resources/icon/colors.h>


class WatchFace {
private:
    CHSV digitColor{ 0, 0, 255 };
    CRGB* leds;

    virtual void initiateTransitionTo(uint8_t nextModeIndex) = 0;
    virtual void performTransition() = 0;

protected:
    bool isTransitioning = false;
    int8_t transitionOffset = 0;

public:
    WatchFace(CRGB* leds) : leds(leds) {}
    ~WatchFace() {}

    virtual unsigned long getUpdateDataPeriod() = 0;
    virtual void setLastTimeDataUpdate(unsigned long updateTime) = 0;
    virtual unsigned long getLastTimeDataUpdate() = 0;
    virtual void resetMode() = 0;

    virtual void showFrame(int16_t xOffset = 0) = 0;
    virtual void nextMode() = 0;
    virtual void updateData() = 0;

    void setDigit(Position2D position2D, uint8_t digit);
    void setColon(bool turnOn, Position2D offsetPosition);
    void setPixel(Position2D position2D);
    void setPixel(Position2D position2D, CHSV color);
    void setIcon(Position2D position2D, const uint8_t(&icon)[8][7], bool isMonotone);
};

#endif 