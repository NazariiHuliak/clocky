#ifndef TIMERWATCHFACE_H
#define TIMERWATCHFACE_H

#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/Transitionable.h>
#include <../src/data/buttons/ButtonHandler.h>
#include <../src/core/model/MinuteSecond.h>

class TimerWatchFace final : public WatchFace {
    /**
     * Modes: <br>
     * 0) show initial time <br>
     * 1) show frequently used time settings <br>
     * 2) seconds: precise set <br>
     * 3) minute: precise set <br>
     * 4) countdown <br>
     * 5) paused <br>
     */
    static constexpr uint8_t numModes = 5;
    uint8_t currentMode = 0;

    // Blinking settings
    bool isVisible = true;
    unsigned long lastShowedTime = 0;
    static constexpr unsigned long showingTime = 500;

    // Timer data
    MinuteSecond currentData = {0, 0};
    ButtonHandler &buttonHandler;

    // Common timers
    int8_t currentCommonTimer = -1;
    bool commonTimerInitializing = true;
    static constexpr uint8_t numCommonTimers = 5;
    MinuteSecond commonTimers[numCommonTimers] = {
        {0, 5}, {5, 0}, {10, 0}, {20, 0}, {40, 0}
    };

    // Update intervals
    static constexpr unsigned long updateDataPeriod = 1000;
    unsigned long lastDataUpdate = 0;
    static constexpr unsigned long buttonsCheckPeriod = 100;
    unsigned long lastButtonsCheck = 0;

    // Display helpers
    void showMinutes(bool blinking, int16_t xOffset) const;
    void showSeconds(bool blinking, int16_t xOffset) const;
    void showColon(bool blinking, int16_t xOffset) const;

    // Button handling
    void checkButtons();
    void sideButtonClick(bool isRight);
    void middleButtonClick();
    void middleButtonHold();
    void changeCurrentCommonTimer(bool direction);

    // Timer logic
    void decrementTime();

public:
    TimerWatchFace(CRGB *leds, ButtonHandler &buttons);
    ~TimerWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override;
    unsigned long getUpdateDataPeriod() override;
    unsigned long getLastTimeDataUpdate() override;

    void nextMode() override;
    void showFrame(int16_t xOffset) override;
    void resetMode() override;
    bool isExternalUpdateAllowed() override;
    void updateData(unsigned long updateTime) override;
};

#endif
