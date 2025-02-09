#ifndef PHOTORESISTORHANDLER_H
#define PHOTORESISTORHANDLER_H

#include <Arduino.h>

class PhotoresistorHandler {
private:
    uint8_t pin;              
    unsigned long lastReadTime = 0;

    static const int size = 10; 
    int rawData[size];
    int index = 0;

    // adjustments
    const uint16_t maxSensorValue = 3000;
    const uint16_t minSensorValue = 320;
    const uint16_t maxBrightness = 6; // from 0-255
    const uint16_t minBrightness = 2; // from 0-255
    float alpha = (maxBrightness - minBrightness) / (float) (maxSensorValue - minSensorValue);

    float getLastAvgRawValue(); 
    float toBrigtness(float rawValue);
public:
    PhotoresistorHandler(uint8_t sensorPin);
    void checkBrightness();
    bool isMeasurementsFinished();
    uint8_t getBrightness();
};

#endif