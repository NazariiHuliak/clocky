#ifndef TIMEWATCHFACE_H
#define TIMEWATCHFACE_H

#include <iarduino_RTC.h>

#include <../src/hardware/time/TimeFetcher.h>
#include <../src/watchface/base/WatchFace.h>


class TimeWatchFace : public WatchFace {
private:
    // modes 
    const uint8_t numModes = 2;
    const uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;
    uint8_t nextModeIndex = 0;

    // data
    TimeFetcher timeFetcher;
    uint8_t* lastData[2];

    // update
    const unsigned long updateDataPeriod = 1000; // in ms
    unsigned long lastDataUpdate = 0;

    void initiateTransitionTo(uint8_t index) override{
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
    TimeWatchFace(CRGB* leds, iarduino_RTC& clock) : WatchFace(leds), timeFetcher(clock) {}

    ~TimeWatchFace() {
        for (uint8_t*& data : lastData) {
            delete[] data;
            data = nullptr;
        }
    }

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
        this->setDigit(Position2D(7 + xOffset, 1 + yOffset), lastData[mode][0] / 10);
        this->setDigit(Position2D(11 + xOffset, 1 + yOffset), lastData[mode][0] % 10);
        this->setDigit(Position2D(17 + xOffset, 1 + yOffset), lastData[mode][1] / 10);
        this->setDigit(Position2D(21 + xOffset, 1 + yOffset), lastData[mode][1] % 10);

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
        lastData[0] = timeFetcher.getCurrentTime();
        lastData[1] = timeFetcher.getCurrentDate();
    }

    void resetMode() override {
        currentModeIndex = mainMode;
    }
};

#endif