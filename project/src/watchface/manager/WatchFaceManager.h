#ifndef WATCHFACE_MANAGER_H
#define WATCHFACE_MANAGER_H

#include <Arduino.h>
#include <FastLED.h>

#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/TransitionableWithDirection.h>


class WatchFaceManager final: public TransitionableWithDirection {
    WatchFace** watchFaces;
    uint8_t m_count;
    uint8_t currentWatchFace = 3;

    bool isWatchFaceChangeAllowed = true;

    unsigned long lastTimeDataUpdate = 0;
    unsigned long checkUpdatePeriod = 100;

    bool isUpdateDataAllowed() const;
public:
    WatchFaceManager(WatchFace** watchFaces, uint8_t count);
    ~WatchFaceManager() override;

    bool getIsWatchFaceChangeAllowed() const;
    void update();
    void updateWatchFacesData();

    void showWatchFace();
    void previousWatchFace();
    void nextMode() const;
    void nextWatchFace();
    void resetCurrentWatchFace() const;

    void initiateTransition(bool direction) override;
    void performTransition() override;
};

#endif