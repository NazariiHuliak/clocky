#include "PhotoresistorDataFetcher.h"


PhotoresistorDataFetcher::PhotoresistorDataFetcher(const uint8_t sensorPin) {
    pin = sensorPin;

    for (unsigned short &i: rawData) {
        i = 0;
    }
}

float PhotoresistorDataFetcher::getLastAvgRawValue() {
    long sum = 0;
    for (const unsigned short i: rawData) {
        sum += i;
    }
    return static_cast<float>(sum) / static_cast<float>(size);
}

uint8_t PhotoresistorDataFetcher::toBrightness(const float rawValue) const {
    if (rawValue <= static_cast<float>(minSensorValue)) return minBrightness;
    if (rawValue >= static_cast<float>(maxSensorValue)) return maxBrightness;
    return static_cast<uint8_t>(alpha * (rawValue - static_cast<float>(minSensorValue)) +
        static_cast<float>(minBrightness));
}

void PhotoresistorDataFetcher::readValue() {
    const int rawValue = analogRead(pin);

    rawData[index] = rawValue;
    index = (index + 1) % size;
}

bool PhotoresistorDataFetcher::isMeasurementsFinished() const {
    return index == size - 1;
}

uint8_t PhotoresistorDataFetcher::getBrightness() {
    return toBrightness(getLastAvgRawValue());
}