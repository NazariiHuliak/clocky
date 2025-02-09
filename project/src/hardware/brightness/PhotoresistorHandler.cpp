#include "PhotoresistorHandler.h"

PhotoresistorHandler::PhotoresistorHandler(const uint8_t sensorPin) {
    pin = sensorPin;

    for (int i = 0; i < size; i++) {
        rawData[i] = 0;
    }
}

float PhotoresistorHandler::getLastAvgRawValue() {
    long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += rawData[i];
    }
    return sum / (float)size;
}

float PhotoresistorHandler::toBrigtness(float rawValue) {
    if (rawValue <= minSensorValue) return minBrightness;
    if (rawValue >= maxSensorValue) return maxBrightness;
    return alpha * (rawValue - minSensorValue) + minBrightness;
}

void PhotoresistorHandler::checkBrightness() {
    unsigned long currentTime = millis();

    if (currentTime - lastReadTime >= 200) {
        lastReadTime = currentTime;

        int rawValue = analogRead(pin);
        Serial.print("Raw value: "); Serial.println(rawValue);
        
        rawData[index] = rawValue;
        index = (index + 1) % size;
    }
}

bool PhotoresistorHandler::isMeasurementsFinished() {
    return index == (size - 1);
}

uint8_t PhotoresistorHandler::getBrightness() {
    return toBrigtness(getLastAvgRawValue());
}