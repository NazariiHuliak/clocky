#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <Arduino.h>
#include <FastLED.h>
#include <iarduino_RTC.h>

#include <../src/model/watchface/Watchface.h>

class LedMatrix {
public:
    LedMatrix();
    ~LedMatrix();
    
    void setWatchFace(WatchFace* watchface, boolean modeWasChanged);
};

#endif