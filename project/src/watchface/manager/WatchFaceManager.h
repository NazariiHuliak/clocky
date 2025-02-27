#ifndef WATCHFACE_MANAGER_H
#define WATCHFACE_MANAGER_H

#include <Arduino.h>
#include <FastLED.h>

#include <../src/watchface/core/WatchFace.h>
#include <../src/watchface/core/transition/TransitionableWithDirection.h>
#include <../src/utils/matrix/MatrixUtil.h>
#include <../src/hardware/brightness/PhotoresistorHandler.h>


class WatchFaceManager : public TransitionableWithDirection {
private:
    WatchFace** watchfaces;
    uint8_t m_count;
    uint8_t currentWatchFace = 0;

    bool isWatchFaceChangeAllowed = true;

    // check data update
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

    void initiateBrightnessChangeTo(uint16_t newBrightness);
    void performBrightnessTransition();

    bool isUpdateDataAllowed();
public:
    WatchFaceManager(WatchFace** watchFaces, uint8_t count);
    ~WatchFaceManager();

    bool getIsWatchFaceChangeAllowed();
    void updateAll();
    void updateWatchFacesData();
    void updateBrightnessData();

    void showWatchFace();
    void previousWatchFace();
    void nextMode();
    void nextWatchFace();
    void resetCurrentWatchFace();

    void initiateTransition(bool direction) override;
    void performTransition() override;
};

#endif