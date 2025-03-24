#ifndef AIRALERT_H
#define AIRALERT_H
#include "core/model/network/AirAlert.h"
#include "watchface/core/WatchFace.h"

class AirAlertWatchFace final: public WatchFace {
    /**
     * Modes: <br>
     * 1) Is air alert active?<br>
     * 2) When air alert was started / when last time finished?<br>
     */
    static constexpr uint8_t numModes = 2;
    static constexpr uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;

    // Data
    AirAlert airAlert;

    // scroll
    int16_t textOffset = 0;
    unsigned long lastScrollTime = 0;
    const unsigned long scrollDelay = 200;  // Adjust speed in milliseconds

    // Update
    const unsigned long updateDataPeriod = 30000;
    unsigned long lastDataUpdate = 0;
public:
    AirAlertWatchFace(CRGB* leds);
    ~AirAlertWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override;
    unsigned long getUpdateDataPeriod() override;
    unsigned long getLastTimeDataUpdate() override;

    void showFrame(int16_t xOffset) override;
    void nextMode() override;
    void resetMode() override;
    bool isExternalUpdateAllowed() override;
    void updateData(unsigned long updateTime) override;
};

#endif
