#ifndef WATCHFACE_MANAGER_H
#define WATCHFACE_MANAGER_H

#include <Arduino.h>
#include <FastLED.h>
#include <iarduino_RTC.h>

#include <../src/watchface/base/WatchFace.h>
#include <../src/utils/matrix/MatrixUtil.h>
#include <../src/hardware/brightness/PhotoresistorHandler.h>

class WatchFaceManager {
private:
    WatchFace** watchfaces;
    uint8_t m_count;
    uint8_t currentWatchFace = 0;

    bool isWatchFaceChangeAllowed = true;

    // data update
    unsigned long lastTimeDataUpdate = 0;
    unsigned long checkUpdatePeriod = 100;

    // brightness 
    PhotoresistorHandler lightHandler;
    uint16_t currentBrightness = 2;
    uint16_t targetBrigthness = 0;
    unsigned long lastBrightnessCheck = 0;
    unsigned long checkBrightnessPeriod = 5000;

    bool isBrightnessTransitioning = false;
    unsigned long lastBrightnessChange = 0;

    int8_t transitionOffset = 0;
    bool isTransitioning = false;
    bool transitionDirection = false;

    void initiateTransition(bool direction);
    void performSlideTransition();

    void initiateBrightnessChangeTo(uint16_t newBrightness);
    void performBrightnessTransition();
public:
    WatchFaceManager(WatchFace** watchFaces, uint8_t count);
    ~WatchFaceManager();

    bool getIsWatchFaceChangeAllowed();

    void updateAll();
    void updateWatchFacesData(unsigned long currentTime);
    void updateBrightnessData(unsigned long currentTime);

    void showWatchFace();
    void previousWatchFace();
    void nextMode();
    void nextWatchFace();
};

#endif