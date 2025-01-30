#ifndef TEMPERATUREWATCHFACE_H
#define TEMPERATUREWATCHFACE_H

#include <../src/watchface/base/WatchFace.h>

class TemperatureWatchFace : public WatchFace {
private:
    const uint8_t numModes = 1;
    const uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;
    uint8_t nextModeIndex = 0;

    const unsigned long updateDataPeriod = 10000; // in ms
    unsigned long lastDataUpdate = 0;

    void initiateTransitionTo(uint8_t index) override {}
    void performTransition() override {}

public:
    TemperatureWatchFace(CRGB* leds) : WatchFace(leds) {}

    ~TemperatureWatchFace() {}

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
        
        this->setDigit(Position2D(7 + xOffset, 1), 2);
        this->setDigit(Position2D(11 + xOffset, 1), 0);
        
        setIcon(Position2D(xOffset, 0), sun);
    }

    void nextMode() override {}

    void updateData() override {
        Serial.println("[info]: TimeWatchFace: updated");
    }

    void resetMode() override {}
}

#endif