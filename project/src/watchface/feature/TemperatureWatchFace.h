#ifndef TEMPERATUREWATCHFACE_H
#define TEMPERATUREWATCHFACE_H

#include <OneWire.h>
#include <DallasTemperature.h>

#include <../src/watchface/core/WatchFace.h>
#include <../src/resources/icon/signs.h>


class TemperatureWatchFace final : public WatchFace {
    // modes 
    const uint8_t numModes = 1;

    // data
    DallasTemperature& temperatureSensor;
    float lastData = 0.0;

    // update
    const unsigned long updateDataPeriod = 10000; // in ms
    unsigned long lastDataUpdate = 0;

public:
    TemperatureWatchFace(CRGB* leds, DallasTemperature& tempSensor) :
        WatchFace(leds),
        temperatureSensor(tempSensor) {
    }

    ~TemperatureWatchFace() override = default;

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
        uint16_t tempValue = lastData * 10;
        drawer.setDigit(Position2D(7 + xOffset, 1), tempValue / 100);
        drawer.setDigit(Position2D(11 + xOffset, 1), (tempValue % 100) / 10);
        drawer.setPixel(Position2D(15 + xOffset, 5));
        drawer.setDigit(Position2D(17 + xOffset, 1), tempValue % 10);

        drawer.setIcon(Position2D(xOffset, 0), sun, false); // Should be change to appropriate
        drawer.setIcon(Position2D(21 + xOffset, 2), celsiusSignSmall, true);
    }

    //TODO: Showing humidity. Change the temperature sensor to DHT** to have the humidity as well.
    void nextMode() override {}

    void resetMode() override {}

    bool isExternalUpdateAllowed() override { return true; }

    void updateData(unsigned long updateTime) override {
        lastDataUpdate = updateTime;
        Serial.println("Temperature updated");

        temperatureSensor.requestTemperatures();
        lastData = temperatureSensor.getTempCByIndex(0);
    }
};

#endif