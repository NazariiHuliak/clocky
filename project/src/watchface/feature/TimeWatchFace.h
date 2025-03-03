#ifndef TIMEWATCHFACE_H
#define TIMEWATCHFACE_H

#include <RtcDS1302.h>

#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/Transitionable.h>
#include <resources/icon/weather.h>
#include <utils/matrix/MatrixUtil.h>


class TimeWatchFace final : public WatchFace, public Transitionable {
    // modes 
    const uint8_t numModes = 2;
    const uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;

    // data
    RtcDS1302<ThreeWire>& clock;
    RtcDateTime now;

    // update
    const unsigned long updateDataPeriod = 1000; // in ms
    unsigned long lastDataUpdate = 0;

    void initiateTransition(uint8_t index) override {
        nextValue = index;
        isTransitioning = true;
        transitionOffset = 0;
    }

    void performTransition() override {
        FastLED.clear();

        showFrame(currentModeIndex, 0, -transitionOffset);
        showFrame(nextValue, 0, -height - transitionOffset);

        transitionOffset -= 1;

        if (transitionOffset < -height) {
            isTransitioning = false;
            transitionOffset = 0;
            currentModeIndex = nextValue;
        }
    }

public:
    TimeWatchFace(CRGB* leds, RtcDS1302<ThreeWire>& rtc) : WatchFace(leds), clock(rtc) {}

    ~TimeWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override {
        return true;
    }

    unsigned long getUpdateDataPeriod() override {
        return updateDataPeriod;
    }

    unsigned long getLastTimeDataUpdate() override {
        return lastDataUpdate;
    }

    void showFrame(const int16_t xOffset) override {
        if (lastDataUpdate == 0) return;

        if (isTransitioning) {
            performTransition();
        } else {
            showFrame(currentModeIndex, xOffset, 0);
        }
    }

    void showFrame(uint8_t mode, int16_t xOffset = 0, int16_t yOffset = 0) const {
        uint8_t firstDigit = mode == 0 ? now.Hour() : now.Day();
        uint8_t secondDigit = mode == 0 ? now.Minute() : now.Month();

        drawer.setDigit(Position2D(7 + xOffset, 1 + yOffset), firstDigit / 10);
        drawer.setDigit(Position2D(11 + xOffset, 1 + yOffset), firstDigit % 10);
        drawer.setDigit(Position2D(17 + xOffset, 1 + yOffset), secondDigit / 10);
        drawer.setDigit(Position2D(21 + xOffset, 1 + yOffset), secondDigit % 10);

        if (currentModeIndex == 0) {
            drawer.setColon(true, Position2D(xOffset, yOffset));
            drawer.setPixel(Position2D(15 + xOffset, 5 + yOffset), CHSV(0, 0, 0));
        } else {
            drawer.setColon(false, Position2D(xOffset, yOffset));
            drawer.setPixel(Position2D(15 + xOffset, 5 + yOffset));
        }

        drawer.setIcon(Position2D(xOffset, 0), sun, false);
    }

    void nextMode() override {
        uint8_t nextModeIndex = (currentModeIndex + 1) % numModes;
        initiateTransition(nextModeIndex);
    }

    void resetMode() override {
        currentModeIndex = mainMode;
    }

    bool isExternalUpdateAllowed() override { return true; }

    void updateData(unsigned long updateTime) override {
        now = clock.GetDateTime();
        lastDataUpdate = updateTime;
    }
};

#endif