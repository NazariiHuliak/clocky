#ifndef TIMEWATCHFACE_H
#define TIMEWATCHFACE_H

#include <../src/watchface/base/WatchFace.h>


class TimeWatchFace : public WatchFace {
private:
    // modes 
    const uint8_t numModes = 2;
    const uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;
    uint8_t nextModeIndex = 0;

    // data
    RtcDS1302<ThreeWire>& clock;
    RtcDateTime now;
    
    // update
    const unsigned long updateDataPeriod = 1000; // in ms
    unsigned long lastDataUpdate = 0;

    void initiateTransitionTo(uint8_t index) override {
        nextModeIndex = index;
        isTransitioning = true;
        transitionOffset = 0;
    }

    void performTransition() override {
        FastLED.clear();

        showFrame(currentModeIndex, 0, -transitionOffset);
        showFrame(nextModeIndex, 0, -height - transitionOffset);

        transitionOffset -= 1;

        if (transitionOffset < -height) {
            isTransitioning = false;
            transitionOffset = 0;
            currentModeIndex = nextModeIndex;
        }
    }

public:
    TimeWatchFace(CRGB* leds, RtcDS1302<ThreeWire>& rtc) : WatchFace(leds), clock(rtc) {}

    ~TimeWatchFace() {}

    unsigned long getUpdateDataPeriod() override {
        return updateDataPeriod;
    }

    void setLastTimeDataUpdate(unsigned long updateTime) override {
        lastDataUpdate = updateTime;
    }

    unsigned long getLastTimeDataUpdate() override {
        return lastDataUpdate;
    }

    void showFrame(int16_t xOffset = 0) override {
        if (lastDataUpdate == 0) return;

        if (isTransitioning) {
            performTransition();
        } else {
            showFrame(currentModeIndex, xOffset, 0);
        }
    }

    void showFrame(uint8_t mode, int16_t xOffset = 0, int16_t yOffset = 0) {
        uint8_t firstDigit = mode == 0 ? now.Hour() : now.Day();
        uint8_t secondDigit = mode == 0 ? now.Minute() : now.Month();

        this->setDigit(Position2D(7 + xOffset, 1 + yOffset), firstDigit / 10);
        this->setDigit(Position2D(11 + xOffset, 1 + yOffset), firstDigit % 10);
        this->setDigit(Position2D(17 + xOffset, 1 + yOffset), secondDigit / 10);
        this->setDigit(Position2D(21 + xOffset, 1 + yOffset), secondDigit % 10);

        if (currentModeIndex == 0) {
            setColon(true, Position2D(xOffset, yOffset));
            this->setPixel(Position2D(15 + xOffset, 5 + yOffset), CHSV(0, 0, 0));
        } else {
            setColon(false, Position2D(xOffset, yOffset));
            this->setPixel(Position2D(15 + xOffset, 5 + yOffset));
        }

        setIcon(Position2D(xOffset, 0), sun, false);
    }

    void nextMode() override {
        uint8_t nextModeIndex = (currentModeIndex + 1) % numModes;
        initiateTransitionTo(nextModeIndex);
    }

    void updateData() override {
        now = clock.GetDateTime();
    }

    void resetMode() override {
        currentModeIndex = mainMode;
    }
};

#endif