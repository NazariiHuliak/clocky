#ifndef STOPWATCHEWATCHFACE_H
#define STOPWATCHEWATCHFACE_H

#include <../src/watchface/base/WatchFace.h>

class StopwatchWatchFace : public WatchFace {
private:
    // update
    const unsigned long updateDataPeriod = 1000; // in ms
    unsigned long lastDataUpdate = 0;

    void initiateTransitionTo(uint8_t index) override {}
    void performTransition() override {}
public:
    StopwatchWatchFace(CRGB* leds) : WatchFace(leds) {}
    ~StopwatchWatchFace() {}

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
        setIcon(Position2D(xOffset, 0), stopwatch, false);
    }

    void nextMode() override {}

    void updateData() override {}

    void resetMode() override {}
};

#endif