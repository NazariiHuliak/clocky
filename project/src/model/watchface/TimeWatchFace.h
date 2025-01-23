#ifndef TIMEWATCHFACE_H
#define TIMEWATCHFACE_H

#include "WatchFace.h"

#include <iarduino_RTC.h>

#include <../src/sensors/time/TimeFetcher.h>


class TimeWatchFace : public WatchFace {
private:
    uint8_t numModes = 2;
    uint8_t currentMode = 0;
    unsigned long lastTimeModeChange = 0;

    TimeFetcher timeFetcher;
public:
    TimeWatchFace(CRGB* leds, iarduino_RTC& clock) : WatchFace(leds), timeFetcher(clock) {}

    void setFrame(bool modeWasChanged) override {
        processModeChange(modeWasChanged);

        uint8_t* time = currentMode == 0 ? timeFetcher.getCurrentTime() : timeFetcher.getCurrentDate();

        this->setDigit(Position2D(7, 1), time[0] / 10);
        this->setDigit(Position2D(11, 1), time[0] % 10);
        this->setDigit(Position2D(17, 1), time[1] / 10);
        this->setDigit(Position2D(21, 1), time[1] % 10);

        setColon(true);
        setIcon();
    }

    void nextMode() override {
        currentMode = (currentMode + 1) % numModes;
    }

    int getNumModes() const override {
        return numModes;
    }

    void setLastTimeModeChange(unsigned long lastTimeModeChange) override {
        this->lastTimeModeChange = lastTimeModeChange;
    }

    unsigned long getLastTimeModeChange() const override {
        return lastTimeModeChange;
    }
};

#endif