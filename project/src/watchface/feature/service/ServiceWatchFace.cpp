#include "ServiceWatchFace.h"

#include "utils/log/Log.h"

ServiceWatchFace::ServiceWatchFace(CRGB *leds): WatchFace(leds) {}

bool ServiceWatchFace::isWatchFaceChangeAllowed() {
    if (shouldBeTransitioned) {
        shouldBeTransitioned = false;
        return true;
    }

    return currentModeIndex == 0;
}

unsigned long ServiceWatchFace::getUpdateDataPeriod() {
    return updateDataPeriod;
}

unsigned long ServiceWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void ServiceWatchFace::showFrame(int16_t xOffset) {
    if (currentModeIndex == 0) {
        // TODO: set icon here or another running string
    } else {
        performRunningString();
    }
}

void ServiceWatchFace::nextMode() {
    if (currentModeIndex == 0) currentModeIndex = 1;
}

void ServiceWatchFace::resetMode() {
    currentModeIndex = 0;
    isAccessPointActive = false;
}

void ServiceWatchFace::setMode(uint8_t mode) {
    currentModeIndex = mode;
}

bool ServiceWatchFace::isExternalUpdateAllowed() {
    return currentModeIndex == 0;
}

void ServiceWatchFace::updateData(unsigned long updateTime) {
    //TODO: Optimize this POS
    // isAccessPointActive can be replaced by getShutDown
    if (updateTime - lastDataUpdate < updateDataPeriod) return;
    lastDataUpdate = updateTime;
    if (currentModeIndex != 0) xOffset -= 1;

    if (currentModeIndex != 0 && !isAccessPointActive) {
        isAccessPointActive = true;
        accessPoint.create(currentModeIndex == 2);
    }

    if (accessPoint.getShouldBeShutDown()) {
        accessPoint.shutdown();
        shouldBeTransitioned = true;
        isAccessPointActive = false;
        currentModeIndex = 0;
    }

    if (!isAccessPointActive && !isServerStopped) {
        isServerStopped = true;
        accessPoint.end();
    }
}

void ServiceWatchFace::performRunningString() {
    FastLED.clear();
    drawer.setSentence(Position2D(xOffset, 1), message.c_str());

    if (xOffset < -4 * (int8_t) message.length()) xOffset = 20;
}