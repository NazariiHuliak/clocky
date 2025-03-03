#ifndef TIMERWATCHFACE_H
#define TIMERWATCHFACE_H

#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/Transitionable.h>
#include <../src/hardware/buttons/ButtonHandler.h>
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
    const uint8_t numModes = 5;
    uint8_t currentMode = 0;

    // blinking
    bool isVisible = false;
    unsigned long lastShowedTime = 0;
    unsigned long showingTime = 500;

    // data
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    ButtonHandler &buttonHandler;

    int8_t currentCommonTimer = -1;
    bool commonTimerInitializing = true;
    static constexpr uint8_t numCommonTimers = 5;
    MinuteSecond commonTimers[numCommonTimers] = {
        {0, 5},
        {5, 0},
        {10, 0},
        {20, 0},
        {40, 0}
    };

    // update
    unsigned long updateDataPeriod = 1000;
    unsigned long lastDataUpdate = 0;
    unsigned long buttonsCheckPeriod = 100;
    unsigned long lastButtonsCheck = 0;

    void showMinutes(const bool blinking, const int16_t xOffset) const {
        if (blinking && !isVisible) return;

        drawer.setDigit(Position2D(7 + xOffset, 1), minutes / 10);
        drawer.setDigit(Position2D(11 + xOffset, 1), minutes % 10);
    }

    void showSeconds(const bool blinking, const int16_t xOffset) const {
        if (blinking && !isVisible) return;

        drawer.setDigit(Position2D(17 + xOffset, 1), seconds / 10);
        drawer.setDigit(Position2D(21 + xOffset, 1), seconds % 10);
    }

    void showColon(const bool blinking, const int16_t xOffset) const {
        if (blinking && !isVisible) return;

        drawer.setColon(true, Position2D(xOffset, 0));
    }

    void checkButtons() {
        switch (buttonHandler.processButtons()) {
            case 1:
                sideButtonClick(false);
                break;
            case 2:
                middleButtonClick();
                break;
            case 3:
                sideButtonClick(true);
                break;
            case 5:
                middleButtonHold();
                break;
            default:
                break;
        }
    }

    void sideButtonClick(const bool isRight) {
        switch (currentMode) {
            case 1: changeCurrentCommonTimer(isRight);
                break;
            case 2:
                seconds = (seconds + (isRight ? 1 : 60 - 1)) % 60;
                break;
            case 3:
                minutes = (minutes + (isRight ? 1 : 99 - 1)) % 99;
            break;
            default: break;
        }
    }

    void middleButtonClick() {
        switch (currentMode) {
            case 1:
                if (!commonTimerInitializing) {
                    commonTimerInitializing = true;
                    changeCurrentCommonTimer(true);
                } else {
                    // countdown
                    currentMode = 4;
                }
                break;
            case 2:
                // go to selecting minutes
                currentMode = 3;
                break;
            case 3:
                // minutes and seconds were set. Countdown
                currentMode = 4;
                break;
            case 4:
                // pause
                currentMode = 5;
                break;
            case 5:
                if (minutes == 0 && seconds == 0) {
                    resetMode();
                } else {
                    // resume
                    currentMode = 4;
                }
                break;
            default: break;
        }
    }

    void middleButtonHold() {
        if (currentMode == 4 || currentMode == 5) {
            // resetting mode if countdown or pause
            resetMode();
        } else {
            // go to manual timer set mode
            currentMode = 2;
            minutes = 0;
            seconds = 0;
        }
    }

    void changeCurrentCommonTimer(const bool direction) {
        currentCommonTimer = static_cast<int8_t>(
            (currentCommonTimer + (direction ? 1 : numCommonTimers - 1)) % numCommonTimers);
        minutes = commonTimers[currentCommonTimer].minute;
        seconds = commonTimers[currentCommonTimer].second;
    }

    void decrementTime() {
        if (seconds == 0) {
            if (minutes == 0) {
                currentCommonTimer = -1;
                currentMode = 5; // Pause when countdown reaches 0
            } else {
                minutes--;
                seconds = 59;
            }
        } else {
            seconds--;
        }
    }

public:
    TimerWatchFace(CRGB *leds, ButtonHandler &buttons) : WatchFace(leds), buttonHandler(buttons) {
    }

    ~TimerWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override {
        return currentMode == 0;
    }

    unsigned long getUpdateDataPeriod() override {
        if (currentMode == 0) {
            return updateDataPeriod;
        }
        return buttonsCheckPeriod;
    }

    unsigned long getLastTimeDataUpdate() override {
        return lastDataUpdate;
    }

    void nextMode() override {
        // setting up the frequently used timers
        currentMode = 1;
        commonTimerInitializing = true;
        changeCurrentCommonTimer(true);
    }

    void showFrame(const int16_t xOffset) override {
        if (!isVisible) FastLED.clear();

        showMinutes(currentMode == 3 || currentMode == 5, xOffset);
        showSeconds(currentMode == 2 || currentMode == 5, xOffset);
        showColon(currentMode == 5, xOffset);

        drawer.setIcon(Position2D(xOffset, 0), timer, false);
    }

    void resetMode() override {
        minutes = 0;
        seconds = 0;

        currentCommonTimer = -1; // not selected
        currentMode = 0; // allows to go to another watchface
        commonTimerInitializing = false;
    }

    bool isExternalUpdateAllowed() override {
        return false;
    }

    void updateData(const unsigned long updateTime) override {
        if (currentMode != 0) {
            checkButtons();

            if (currentMode == 4) {
                if (updateTime - lastDataUpdate >= updateDataPeriod) {
                    decrementTime();
                    lastDataUpdate = updateTime;
                }
            } else if (currentMode != 1) {
                if (updateTime - lastShowedTime >=  showingTime) {
                    isVisible = !isVisible;
                    lastShowedTime = updateTime;
                }
            }
        }
    }
};

#endif