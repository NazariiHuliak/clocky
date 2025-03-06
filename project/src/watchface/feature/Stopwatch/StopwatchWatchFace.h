#ifndef STOPWATCHWATCHFACE_H
#define STOPWATCHWATCHFACE_H

#include <../src/watchface/core/WatchFace.h>
#include <../src/resources/icon/watchface.h>

class StopwatchWatchFace final : public WatchFace {
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

    void incrementTime();

public:
    explicit StopwatchWatchFace(CRGB* leds);
    ~StopwatchWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override;
    unsigned long getUpdateDataPeriod() override;
    unsigned long getLastTimeDataUpdate() override;
    void showFrame(const int16_t xOffset) override;
    void nextMode() override;
    void resetMode() override;
    bool isExternalUpdateAllowed() override;
    void updateData(unsigned long updateTime) override;
};

#endif