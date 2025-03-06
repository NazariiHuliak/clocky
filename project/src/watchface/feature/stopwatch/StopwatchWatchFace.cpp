#include "StopwatchWatchFace.h"
#include <Arduino.h>

StopwatchWatchFace::StopwatchWatchFace(CRGB* leds) : WatchFace(leds) {}

bool StopwatchWatchFace::isWatchFaceChangeAllowed() {
    return true;
}

unsigned long StopwatchWatchFace::getUpdateDataPeriod() {
    return updateDataPeriod;
}

unsigned long StopwatchWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void StopwatchWatchFace::showFrame(const int16_t xOffset) {
    if (currentMode == 2 && !displayTime) {
        FastLED.clear();
        drawer.setIcon(Position2D(xOffset, 0), stopwatch, false);
        return;
    }

    drawer.setDigit(Position2D(7 + xOffset, 1), minutes / 10);
    drawer.setDigit(Position2D(11 + xOffset, 1), minutes % 10);
    drawer.setDigit(Position2D(17 + xOffset, 1), seconds / 10);
    drawer.setDigit(Position2D(21 + xOffset, 1), seconds % 10);

    drawer.setColon(true, Position2D(xOffset, 0));
    drawer.setIcon(Position2D(xOffset, 0), stopwatch, false);
}

void StopwatchWatchFace::nextMode() {
    currentMode = (currentMode == 2) ? 1 : currentMode + 1;
}

void StopwatchWatchFace::resetMode() {
    currentMode = 0;
    minutes = 0;
    seconds = 0;
}

bool StopwatchWatchFace::isExternalUpdateAllowed() {
    return currentMode == 0;
}

void StopwatchWatchFace::updateData(unsigned long updateTime) {
    lastDataUpdate = updateTime;
    if (currentMode == 2) {
        displayTime = !displayTime;
        return;
    } else if (currentMode == 0) return;

    incrementTime();
}

void StopwatchWatchFace::incrementTime() {
    if (seconds + 1 > 59) {
        if (minutes + 1 > 99) {
            currentMode = 2;
        } else {
            minutes++;
        }
        seconds = 0;
    } else {
        seconds++;
    }
}