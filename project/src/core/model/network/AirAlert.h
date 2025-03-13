#ifndef AIRALERTDATA_H
#define AIRALERTDATA_H

#include <Arduino.h>

struct AirAlert {
    bool alertActive = false;
    unsigned long lastUpdate = 0;

    void print() {
        Serial.print(F("AirAlert { alertActive: "));
        Serial.print(alertActive ? F("true") : F("false"));
        Serial.print(F(", lastUpdate: "));
        Serial.print(lastUpdate);
        Serial.println(F(" }"));
    }
};

#endif
