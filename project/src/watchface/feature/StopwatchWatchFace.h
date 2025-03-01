#ifndef STOPWATCHEWATCHFACE_H
#define STOPWATCHEWATCHFACE_H

#include <../src/watchface/core/WatchFace.h>


class StopwatchWatchFace : public WatchFace {
private:
    // update
    const unsigned long updateDataPeriod = 1000; // in ms
    unsigned long lastDataUpdate = 0;

    uint8_t minutes = 0;
    uint8_t seconds = 0;

    /**
     * Modes:
     * 0) show zeros
     * 1) time from start stopwatch
     * 2) paused
    */
    const uint8_t numModes = 3;
    uint8_t currentMode = 0;

    bool displayTime = true;

    void incrementTime() {
        if (seconds + 1 > 59) {
            if (minutes + 1 > 99) {
                currentMode = 2;
            } else {
                minutes++;
            }
            seconds = 0;
        } else {
            seconds++;
        }
    }
public:
    StopwatchWatchFace(CRGB* leds) : WatchFace(leds) {}
    ~StopwatchWatchFace() {}

    unsigned long getUpdateDataPeriod() override {
        return updateDataPeriod;
    }

    unsigned long getLastTimeDataUpdate() override {
        return lastDataUpdate;
    }

    void showFrame(int16_t xOffset = 0) override {
        if (currentMode == 2 && !displayTime) {
            FastLED.clear();
            setIcon(Position2D(xOffset, 0), stopwatch, false);
            return;
        }

        this->setDigit(Position2D(7 + xOffset, 1), minutes / 10);
        this->setDigit(Position2D(11 + xOffset, 1), minutes % 10);
        this->setDigit(Position2D(17 + xOffset, 1), seconds / 10);
        this->setDigit(Position2D(21 + xOffset, 1), seconds % 10);

        setColon(true, Position2D(xOffset, 0));
        setIcon(Position2D(xOffset, 0), stopwatch, false);
    }

    void nextMode() override {
        currentMode = (currentMode == 2) ? 1 : currentMode + 1;
    }

    void resetMode() override {
        currentMode = 0;
        minutes = 0;
        seconds = 0;
    }

    bool isUpdateAllowed() override {
        return currentMode == 0;
    }

    void updateData(unsigned long updateTime) override {
        lastDataUpdate = updateTime;
        if (currentMode == 2) {
            displayTime = !displayTime;
            return;
        } else if (currentMode == 0) return;

        incrementTime();
    }
};

#endif