#include "StopwatchWatchFace.h"
#include <Arduino.h>

#include "utils/log/Log.h"

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
    if (currentModeIndex == 2 && !displayTime) {
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
    currentModeIndex = (currentModeIndex == 2) ? 1 : currentModeIndex + 1;
}

void StopwatchWatchFace::resetMode() {
    currentModeIndex = 0;
    minutes = 0;
    seconds = 0;
}

bool StopwatchWatchFace::isExternalUpdateAllowed() {
    return currentModeIndex == 0;
}

void StopwatchWatchFace::updateData(unsigned long updateTime) {
    lastDataUpdate = updateTime;
    if (currentModeIndex == 2) {
        displayTime = !displayTime;
        return;
    } else if (currentModeIndex == 0) return;

    incrementTime();
    Log::info("StopwatchWatchFace: data updated at ", String(updateTime), ".");
}

void StopwatchWatchFace::incrementTime() {
    if (seconds + 1 > 59) {
        if (minutes + 1 > 99) {
            currentModeIndex = 2;
        } else {
            minutes++;
        }
        seconds = 0;
    } else {
        seconds++;
    }
}

void StopwatchWatchFace::setMode(uint8_t mode) {
    currentModeIndex = mode;
}