#ifndef STOPWATCHEWATCHFACE_H
#define STOPWATCHEWATCHFACE_H

#include <../src/watchface/core/WatchFace.h>
#include <../src/resources/icon/watchface.h>


class StopwatchWatchFace final : public WatchFace {
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
    explicit StopwatchWatchFace(CRGB* leds) : WatchFace(leds) {}

    ~StopwatchWatchFace() override = default;

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
        if (currentMode == 2 && !displayTime) {
            FastLED.clear();
            drawer.setIcon(Position2D(xOffset, 0), stopwatch, false);
            return;
        }

        drawer.setDigit(Position2D(7 + xOffset, 1), minutes / 10);
        drawer.setDigit(Position2D(11 + xOffset, 1), minutes % 10);
        drawer.setDigit(Position2D(17 + xOffset, 1), seconds / 10);
        drawer.setDigit(Position2D(21 + xOffset, 1), seconds % 10);

        drawer.setColon(true, Position2D(xOffset, 0));
        drawer.setIcon(Position2D(xOffset, 0), stopwatch, false);
    }

    void nextMode() override {
        currentMode = (currentMode == 2) ? 1 : currentMode + 1;
    }

    void resetMode() override {
        currentMode = 0;
        minutes = 0;
        seconds = 0;
    }

    bool isExternalUpdateAllowed() override {
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