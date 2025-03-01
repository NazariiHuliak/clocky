#ifndef BRIGHTNESSHANDLER_H
#define BRIGHTNESSHANDLER_H

#include <Arduino.h>
#include <FastLED.h>
#include "../src/hardware/brightness/Photoresistor/PhotoresistorDataFetcher.h"
#include <../src/core/transition/Transitionable.h>


class BrightnessHandler final : public Transitionable {
    PhotoresistorDataFetcher photoresistor;
    uint16_t currentBrightness;

    // update
    unsigned long lastReadTime = 0;
    unsigned long readPeriod = 200;
    unsigned long lastBrightnessCheck = 0;
    unsigned long checkBrightnessPeriod = 5000;

    // smooth change
    unsigned long lastBrightnessChange = 0;

    void initiateTransition(const uint8_t nextValue_) override {
        isTransitioning = true;
        nextValue = nextValue_;
    }

    void performTransition() override {
        nextValue > currentBrightness ? currentBrightness++ : currentBrightness--;
        if (currentBrightness == nextValue) isTransitioning = false;
        FastLED.setBrightness(currentBrightness);
    }

    void checkBrightnessValue(const unsigned long checkBrightnessTime) {
        if (checkBrightnessTime - lastReadTime >= readPeriod) {
            photoresistor.readValue();
        }

        if (photoresistor.isMeasurementsFinished()) {
            lastBrightnessCheck = checkBrightnessTime;

            const uint8_t newBrightness = photoresistor.getBrightness();
            if (currentBrightness != newBrightness) {
                initiateTransition(newBrightness);
            }
        }
    }
public:
    explicit BrightnessHandler(const uint8_t sensorPin, const uint16_t initialBrightness = 2)
      : photoresistor(sensorPin), currentBrightness(initialBrightness) {}

    void update() {
        const unsigned long currentTime = millis();

        if (isTransitioning) {
              if (currentTime - lastBrightnessChange >= 20) {
                  lastBrightnessChange = currentTime;
                  performTransition();
              }
        } else if (currentTime - lastBrightnessCheck >= checkBrightnessPeriod) {
              checkBrightnessValue(currentTime);
        }
    }
};

#endif