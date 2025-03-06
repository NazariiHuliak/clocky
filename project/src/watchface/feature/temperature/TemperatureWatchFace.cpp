#include "TemperatureWatchFace.h"
#include <Arduino.h>

TemperatureWatchFace::TemperatureWatchFace(CRGB* leds, DallasTemperature& tempSensor) :
    WatchFace(leds),
    temperatureSensor(tempSensor) {}

bool TemperatureWatchFace::isWatchFaceChangeAllowed() {
    return true;
}

unsigned long TemperatureWatchFace::getUpdateDataPeriod() {
    return updateDataPeriod;
}

unsigned long TemperatureWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void TemperatureWatchFace::showFrame(const int16_t xOffset) {
    uint16_t tempValue = lastData * 10;
    drawer.setDigit(Position2D(7 + xOffset, 1), tempValue / 100);
    drawer.setDigit(Position2D(11 + xOffset, 1), (tempValue % 100) / 10);
    drawer.setPixel(Position2D(15 + xOffset, 5));
    drawer.setDigit(Position2D(17 + xOffset, 1), tempValue % 10);

    drawer.setIcon(Position2D(xOffset, 0), sun, false); // Should be changed to appropriate
    drawer.setIcon(Position2D(21 + xOffset, 2), celsiusSignSmall, true);
}

//TODO: Showing humidity. Change the temperature sensor to DHT** to have the humidity as well.
void TemperatureWatchFace::nextMode() {}

void TemperatureWatchFace::resetMode() {}

bool TemperatureWatchFace::isExternalUpdateAllowed() {
    return true;
}

void TemperatureWatchFace::updateData(unsigned long updateTime) {
    lastDataUpdate = updateTime;
    Serial.println("Temperature updated");

    temperatureSensor.requestTemperatures();
    lastData = temperatureSensor.getTempCByIndex(0);
}