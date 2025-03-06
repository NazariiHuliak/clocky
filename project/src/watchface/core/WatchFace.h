#ifndef WATCHFACE_H
#define WATCHFACE_H

#include <FastLED.h>

#include <../src/utils/matrix/MatrixUtil.h>
#include <../src/resources/font/digits.h>
#include <../src/resources/icon/weather.h>
#include <../src/resources/icon/colors.h>
#include <../src/watchface/core/PixelDrawer/PixelDrawer.h>

class WatchFace {
protected:
    PixelDrawer drawer;
public:
    explicit WatchFace(CRGB* leds) : drawer(leds) {}
    virtual ~WatchFace() = default;

    virtual bool isWatchFaceChangeAllowed();
    virtual unsigned long getUpdateDataPeriod() = 0;
    virtual unsigned long getLastTimeDataUpdate() = 0;

    virtual void showFrame(int16_t xOffset) = 0;
    virtual void nextMode() = 0;
    virtual void resetMode() = 0;

    virtual bool isExternalUpdateAllowed() = 0;
    virtual void updateData(unsigned long updateTime) = 0;
};

#endif