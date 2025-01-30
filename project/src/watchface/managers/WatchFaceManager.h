#ifndef WATCHFACE_MANAGER_H
#define WATCHFACE_MANAGER_H

#include <Arduino.h>
#include <FastLED.h>
#include <iarduino_RTC.h>

#include <../src/watchface/base/WatchFace.h>
#include <../src/utils/matrix/MatrixUtil.h>

class WatchFaceManager {
private:
    WatchFace** watchfaces;
    uint8_t m_count;
    uint8_t currentWatchFace = 0;

    bool isWatchFaceChangeAllowed = true;

    unsigned long lastTimeDataUpdate = 0;
    unsigned long checkUpdatePeriod = 100;

    int8_t transitionOffset = 0;
    bool isTransitioning = false;
    bool transitionDirection = false;

    void initiateTransition(bool direction);
    void performSlideTransition();
public:
    WatchFaceManager(WatchFace** watchFaces, uint8_t count);
    ~WatchFaceManager();

    bool getIsWatchFaceChangeAllowed();
    void updateWatchFaceData();

    void showWatchFace();
    void previousWatchFace();
    void nextMode();
    void nextWatchFace();
};

#endif