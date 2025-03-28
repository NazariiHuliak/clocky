#ifndef TEMPERATUREWATCHFACE_H
#define TEMPERATUREWATCHFACE_H

#include <OneWire.h>
#include <DallasTemperature.h>

#include <../src/watchface/core/WatchFace.h>
#include <../src/resources/icon/signs.h>

class TemperatureWatchFace final : public WatchFace {
    const uint8_t numModes = 1;

    // Data
    DallasTemperature& temperatureSensor;
    float lastData = 0.0;

    // Update
    const unsigned long updateDataPeriod = 10000; // in ms
    unsigned long lastDataUpdate = 0;

public:
    TemperatureWatchFace(CRGB* leds, DallasTemperature& tempSensor);
    ~TemperatureWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override;
    unsigned long getUpdateDataPeriod() override;
    unsigned long getLastTimeDataUpdate() override;

    void showFrame(int16_t xOffset) override;
    void nextMode() override;
    void resetMode() override;
    bool isExternalUpdateAllowed() override;
    void updateData(unsigned long updateTime) override;
    void setMode(uint8_t mode) override;
};

#endif