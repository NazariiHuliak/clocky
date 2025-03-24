#ifndef TIMEWATCHFACE_H
#define TIMEWATCHFACE_H

#include <RtcDS1302.h>
#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/Transitionable.h>
#include <resources/icon/weather.h>
#include <resources/icon/signs.h>
#include <utils/matrix/MatrixUtil.h>

class TimeWatchFace final : public WatchFace, public Transitionable {
private:
    // Modes
    static constexpr uint8_t numModes = 2;
    static constexpr uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;

    // Data
    RtcDS1302<ThreeWire>& clock;
    RtcDateTime now;

    // Update timing
    static constexpr unsigned long updateDataPeriod = 1000; // in ms
    unsigned long lastDataUpdate = 0;

    // Transition handling
    void initiateTransition(uint8_t index) override;
    void performTransition() override;

    // Display rendering
    void showFrame(uint8_t mode, int16_t xOffset = 0, int16_t yOffset = 0) const;

public:
    TimeWatchFace(CRGB* leds, RtcDS1302<ThreeWire>& rtc);
    ~TimeWatchFace() override = default;

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