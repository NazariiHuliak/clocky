#ifndef WATCHFACE_MANAGER_H
#define WATCHFACE_MANAGER_H

#include <Arduino.h>
#include <FastLED.h>

#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/MultiInitiationTransitionable.h>
#include "../src/utils/log/Log.h"
#include "core/transition/Transitionable.h"

class WatchFaceManager final: public MultiInitiationTransitionable {
    // WatchFaces
    WatchFace** watchFaces;
    uint8_t m_count;
    uint8_t currentWatchFace = 0;

    // Update
    bool isWatchFaceChangeAllowed = true;
    unsigned long lastTimeDataUpdate = 0;
    unsigned long checkUpdatePeriod = 100;

    // AirAlert
    bool wasTransitionedToAirAlert = false;
    uint8_t airAlertWatchFace = 4;
    unsigned long lastAirAlertCheck = 0;

    bool isUpdateDataAllowed() const;
public:
    WatchFaceManager(WatchFace** watchFaces, uint8_t count);
    ~WatchFaceManager() override;

    bool getIsWatchFaceChangeAllowed() const;

    void update();
    void checkAirAlert(unsigned long updateTime);
    void updateWatchFacesData(unsigned long updateTime);

    void showWatchFace();
    void previousWatchFace();
    void nextMode() const;
    void nextWatchFace();
    void resetCurrentWatchFace() const;

    void initiateTransition(uint8_t nextValue_) override;
    void initiateTransition(bool direction) override;
    void performTransition() override;
};

#endif