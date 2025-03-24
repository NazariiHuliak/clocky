#include "TimerWatchFace.h"
#include "../src/resources/icon/watchface.h"
#include <FastLED.h>

TimerWatchFace::TimerWatchFace(CRGB *leds, ButtonHandler &buttons)
    : WatchFace(leds), buttonHandler(buttons) {}

bool TimerWatchFace::isWatchFaceChangeAllowed() {
    return currentMode == 0 || currentMode == 5;
}

unsigned long TimerWatchFace::getUpdateDataPeriod() {
    return (currentMode == 0) ? updateDataPeriod : buttonsCheckPeriod;
}

unsigned long TimerWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void TimerWatchFace::nextMode() {
    // setting up the frequently used timers
    currentMode = 1;
    commonTimerInitializing = true;
    changeCurrentCommonTimer(true);
}

void TimerWatchFace::showFrame(int16_t xOffset) {
    if (!isVisible) FastLED.clear();

    showMinutes(currentMode == 3 || currentMode == 5, xOffset);
    showSeconds(currentMode == 2 || currentMode == 5, xOffset);
    showColon(currentMode == 5, xOffset);

    drawer.setIcon(Position2D(xOffset, 0), timer, false);
}

void TimerWatchFace::resetMode() {
    currentData.minutes = 0;
    currentData.seconds = 0;
    currentCommonTimer = -1;
    currentMode = 0;
    commonTimerInitializing = false;
    isVisible = true;
}

bool TimerWatchFace::isExternalUpdateAllowed() {
    return currentMode == 0;
}

void TimerWatchFace::updateData(unsigned long updateTime) {
    if (currentMode != 0) {
        checkButtons();

        if (currentMode == 4 && updateTime - lastDataUpdate >= updateDataPeriod) {
            decrementTime();
            lastDataUpdate = updateTime;
        } else if (currentMode != 1 && updateTime - lastShowedTime >= showingTime) {
            isVisible = !isVisible;
            lastShowedTime = updateTime;
        }
    }
}

void TimerWatchFace::showMinutes(bool blinking, int16_t xOffset) const {
    if (blinking && !isVisible) return;

    drawer.setDigit(Position2D(7 + xOffset, 1), currentData.minutes / 10);
    drawer.setDigit(Position2D(11 + xOffset, 1), currentData.minutes % 10);
}

void TimerWatchFace::showSeconds(bool blinking, int16_t xOffset) const {
    if (blinking && !isVisible) return;

    drawer.setDigit(Position2D(17 + xOffset, 1), currentData.seconds / 10);
    drawer.setDigit(Position2D(21 + xOffset, 1), currentData.seconds % 10);
}

void TimerWatchFace::showColon(bool blinking, int16_t xOffset) const {
    if (blinking && !isVisible) return;

    drawer.setColon(true, Position2D(xOffset, 0));
}

void TimerWatchFace::checkButtons() {
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

void TimerWatchFace::sideButtonClick(bool isRight) {
    switch (currentMode) {
        case 1:
            changeCurrentCommonTimer(isRight);
            break;
        case 2:
            currentData.seconds = (currentData.seconds + (isRight ? 1 : 59)) % 60;
            break;
        case 3:
            currentData.minutes = (currentData.minutes + (isRight ? 1 : 98)) % 99;
            break;
        default:
            break;
    }
}

void TimerWatchFace::middleButtonClick() {
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
            if (currentData.minutes == 0 && currentData.seconds == 0) {
                resetMode();
            } else {
                // resume
                currentMode = 4;
            }
        break;
        default: break;
    }
}

void TimerWatchFace::middleButtonHold() {
    if (currentMode == 4 || currentMode == 5) {
        // resetting mode if countdown or pause
        resetMode();
    } else {
        // go to manual timer set mode
        currentMode = 2;
        currentData.minutes = 0;
        currentData.seconds = 0;
    }
}

void TimerWatchFace::changeCurrentCommonTimer(bool direction) {
    currentCommonTimer = static_cast<int8_t>(
        (currentCommonTimer + (direction ? 1 : numCommonTimers - 1)) % numCommonTimers);
    currentData = commonTimers[currentCommonTimer];
}

void TimerWatchFace::decrementTime() {
    if (currentData.seconds == 0) {
        if (currentData.minutes == 0) {
            currentCommonTimer = -1;
            currentMode = 5;
        } else {
            currentData.minutes--;
            currentData.seconds = 59;
        }
    } else {
        currentData.seconds--;
    }
}
