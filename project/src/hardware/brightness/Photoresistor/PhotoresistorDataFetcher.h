#ifndef PHOTORESISTORDATAFETCHER_H
#define PHOTORESISTORDATAFETCHER_H

#include <Arduino.h>

class PhotoresistorDataFetcher {
    uint8_t pin;

    //storage
    static constexpr int size = 10;
    uint16_t rawData[size] = {};
    int index = 0;

    // adjustments
    const uint16_t maxSensorValue = 3000;
    const uint16_t minSensorValue = 320;
    const uint16_t maxBrightness = 6; // from 0-255
    const uint16_t minBrightness = 2; // from 0-255
    float alpha = static_cast<float>(maxBrightness - minBrightness) / static_cast<float>(maxSensorValue - minSensorValue);

    float getLastAvgRawValue(); 
    uint8_t toBrightness(float rawValue) const;
public:
    explicit PhotoresistorDataFetcher(uint8_t sensorPin);
    void readValue();
    bool isMeasurementsFinished() const;
    uint8_t getBrightness();
};

#endif