#include "TimeWatchFace.h"
#include <FastLED.h>
#include <../src/data/network/NetworkDataManager.h>

TimeWatchFace::TimeWatchFace(CRGB *leds, RtcDS1302<ThreeWire> &rtc)
    : WatchFace(leds), clock(rtc) {
}

bool TimeWatchFace::isWatchFaceChangeAllowed() {
    return true;
}

unsigned long TimeWatchFace::getUpdateDataPeriod() {
    return updateDataPeriod;
}

unsigned long TimeWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void TimeWatchFace::showFrame(const int16_t xOffset) {
    if (lastDataUpdate == 0) return;

    if (isTransitioning) {
        performTransition();
    } else {
        showFrame(currentModeIndex, xOffset, 0);
    }
}

void TimeWatchFace::showFrame(uint8_t mode, int16_t xOffset, int16_t yOffset) const {
    uint8_t firstDigit = (mode == 0) ? now.Hour() : now.Day();
    uint8_t secondDigit = (mode == 0) ? now.Minute() : now.Month();

    drawer.setDigit(Position2D(7 + xOffset, 1 + yOffset), firstDigit / 10);
    drawer.setDigit(Position2D(11 + xOffset, 1 + yOffset), firstDigit % 10);
    drawer.setDigit(Position2D(17 + xOffset, 1 + yOffset), secondDigit / 10);
    drawer.setDigit(Position2D(21 + xOffset, 1 + yOffset), secondDigit % 10);

    if (currentModeIndex == 0) {
        drawer.setColon(true, Position2D(xOffset, yOffset));
        drawer.setPixel(Position2D(15 + xOffset, 5 + yOffset), CHSV(0, 0, 0));
    } else {
        drawer.setColon(false, Position2D(xOffset, yOffset));
        drawer.setPixel(Position2D(15 + xOffset, 5 + yOffset));
    }

    bool isAirAlert = NetworkDataManager::instance().getAirAlert().alertActive;
    drawer.setIcon(
        Position2D(xOffset, 0),
        isAirAlert ? airAlertSign : sun,
        false);
}

void TimeWatchFace::nextMode() {
    uint8_t nextModeIndex = (currentModeIndex + 1) % numModes;
    initiateTransition(nextModeIndex);
}

void TimeWatchFace::resetMode() {
    currentModeIndex = mainMode;
}

bool TimeWatchFace::isExternalUpdateAllowed() {
    return true;
}

void TimeWatchFace::updateData(unsigned long updateTime) {
    now = clock.GetDateTime();
    lastDataUpdate = updateTime;
    Log::info("TimeWatchFace: data updated at ", String(updateTime), ".");
}

void TimeWatchFace::initiateTransition(uint8_t index) {
    nextValue = index;
    isTransitioning = true;
    transitionOffset = 0;
}

void TimeWatchFace::performTransition() {
    FastLED.clear();

    showFrame(currentModeIndex, 0, -transitionOffset);
    showFrame(nextValue, 0, -MATRIX_HEIGHT - transitionOffset);

    transitionOffset -= 1;

    if (transitionOffset < -MATRIX_HEIGHT) {
        isTransitioning = false;
        transitionOffset = 0;
        currentModeIndex = nextValue;
    }
}
