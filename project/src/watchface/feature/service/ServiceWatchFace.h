#ifndef SERVICEWATCHFACE_H
#define SERVICEWATCHFACE_H
#include "core/transition/Transitionable.h"
#include "data/network/AccessPoint.h"
#include "watchface/core/WatchFace.h"

class ServiceWatchFace : public WatchFace {
    static constexpr uint8_t numModes = 3;
    static constexpr uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;

    // Update timing
    static constexpr unsigned long updateDataPeriod = 100; // in ms
    unsigned long lastDataUpdate = 0;

    // Data
    String message = "Connect to the 192.168.4.1";
    AccessPoint accessPoint;
    bool isAccessPointActive = false;

    bool isServerStopped = false;
    bool shouldBeTransitioned = false;

    // Running string
    int16_t xOffset = 20;
    void performRunningString();
public:
    ServiceWatchFace(CRGB * leds);

    ~ServiceWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override;
    unsigned long getUpdateDataPeriod() override;
    unsigned long getLastTimeDataUpdate() override;
    void showFrame(int16_t xOffset) override;

    void nextMode() override;
    void resetMode() override;
    void setMode(uint8_t mode) override;

    bool isExternalUpdateAllowed() override;
    void updateData(unsigned long updateTime) override;
};

#endif
