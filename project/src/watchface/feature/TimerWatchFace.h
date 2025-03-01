#ifndef TIMERWATCHFACE_H
#define TIMERWATCHFACE_H

#include <../src/watchface/core/WatchFace.h>
#include <../src/core/transition/Transitionable.h>
#include <../src/hardware/buttons/ButtonHandler.h>


class TimerWatchFace : public WatchFace, public Transitionable {
private:
    /**
     * Modes:
     * 0) show initial time
     * 1) countdown
     * 2) paused
     */
    const uint8_t numModes = 3;
    uint8_t currentMode = 0;
    bool displayTime = true;

    //data
    uint8_t minutes = 10;
    uint8_t seconds = 0;
    ButtonHandler& buttonHandler;
    uint8_t commonTimers[4] = {5, 10, 20, 40};

    // update
    const unsigned long updateDataPeriod = 1000;
    unsigned long lastDataUpdate = 0;

    void initiateTransition(uint8_t index) override {}
    void performTransition() override {}

    void decrementTime() {
        if (seconds == 0) {
            if (minutes == 0) {
                currentMode = 2; // Pause when countdown reaches 0
            } else {
                minutes--;
                seconds = 59;
            }
        } else {
            seconds--;
        }
    }
public:
    TimerWatchFace(CRGB* leds, ButtonHandler& buttons) : WatchFace(leds), buttonHandler(buttons) {}

    ~TimerWatchFace() {}

    unsigned long getUpdateDataPeriod() override {
        return updateDataPeriod;
    }

    unsigned long getLastTimeDataUpdate() override {
        return lastDataUpdate;
    }

    void showFrame(int16_t xOffset = 0) override {
        if (currentMode == 2 && !displayTime) {
            FastLED.clear();
            setIcon(Position2D(xOffset, 0), timer, false);
            return;
        }

        this->setDigit(Position2D(7 + xOffset, 1), minutes / 10);
        this->setDigit(Position2D(11 + xOffset, 1), minutes % 10);
        this->setDigit(Position2D(17 + xOffset, 1), seconds / 10);
        this->setDigit(Position2D(21 + xOffset, 1), seconds % 10);

        setColon(true, Position2D(xOffset, 0));
        setIcon(Position2D(xOffset, 0), timer, false);
    }

    void nextMode() override {
        currentMode = (currentMode == 2) ? 1 : currentMode + 1;
    }

    void resetMode() override {
        currentMode = 0;
        displayTime = true;
    }

    bool isUpdateAllowed() override {
        return currentMode == 0;
    }

    void updateData(unsigned long updateTime) override {
        lastDataUpdate = updateTime;
        if (currentMode == 2) {
            displayTime = !displayTime;
            return;
        } else if (currentMode == 0) return;

        decrementTime();
    }
};

#endif