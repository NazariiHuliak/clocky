#include "WatchFaceManager.h"

WatchFaceManager::WatchFaceManager(WatchFace** watchFaces, uint8_t count) : watchfaces(watchFaces), m_count(count), lightHandler(A5) {}

WatchFaceManager::~WatchFaceManager() {
    for (uint8_t i = 0; i < m_count; i++) {
        delete watchfaces[i];
    }
    delete[] watchfaces;
}

bool WatchFaceManager::getIsWatchFaceChangeAllowed() {
    return isWatchFaceChangeAllowed && !isTransitioning;
}

void WatchFaceManager::updateAll() {
    unsigned long currentTime = millis();

    updateWatchFacesData(currentTime);
    // updateBrightnessData(currentTime);
}

void WatchFaceManager::updateWatchFacesData(unsigned long currentTime) {
    if (currentTime - lastTimeDataUpdate >= checkUpdatePeriod) {
        lastTimeDataUpdate = currentTime;

        for (uint8_t i = 0; i < m_count; i++) {
            unsigned long lastTimeUpdate = watchfaces[i]->getLastTimeDataUpdate();
            if (currentTime - lastTimeUpdate >= watchfaces[i]->getUpdateDataPeriod() || lastTimeUpdate == 0) {
                watchfaces[i]->updateData();
                watchfaces[i]->setLastTimeDataUpdate(currentTime);
            }
            if (currentWatchFace != i) {
                watchfaces[i]->resetMode();
            }
        }
    }
}

void WatchFaceManager::updateBrightnessData(unsigned long currentTime) {
    if (currentTime - lastBrightnessCheck > checkBrightnessPeriod) {
        lightHandler.checkBrightness();

        if (lightHandler.isMeasurementsFinished()) {
            lastBrightnessCheck = currentTime;
            uint8_t newBrightness = lightHandler.getBrightness();
            
            if (currentBrightness != newBrightness) {
                initiateBrightnessChangeTo(newBrightness);  
            }
        }
    }
}

void WatchFaceManager::initiateBrightnessChangeTo(uint16_t newBrightness) {
    isBrightnessTransitioning = true;
    targetBrigthness = newBrightness;
}

void WatchFaceManager::showWatchFace() {
    
    // watchfaces[currentWatchFace]->showFrame();
    if (isTransitioning) {
        performSlideTransition();
    } else if (watchfaces[currentWatchFace]) {
        watchfaces[currentWatchFace]->showFrame();
    }

    // if (isBrightnessTransitioning) {
    //     performBrightnessTransition();
    // }
}

void WatchFaceManager::previousWatchFace() {
    if (!isWatchFaceChangeAllowed) return;

    initiateTransition(false);
}

void WatchFaceManager::nextMode() {
    watchfaces[currentWatchFace]->nextMode();
}

void WatchFaceManager::nextWatchFace() {
    if (!isWatchFaceChangeAllowed) return;

    initiateTransition(true);
}

void WatchFaceManager::initiateTransition(bool direction) {
    isTransitioning = true;
    transitionOffset = 0;
    isWatchFaceChangeAllowed = false;
    transitionDirection = direction;
}

void WatchFaceManager::performSlideTransition() {
    FastLED.clear();

    watchfaces[currentWatchFace]->showFrame(-transitionOffset);

    uint8_t nextFaceIndex = (currentWatchFace + (transitionDirection ? 1 : m_count - 1)) % m_count;
    watchfaces[nextFaceIndex]->showFrame(transitionDirection ? width - transitionOffset : -width - transitionOffset);

    transitionOffset += transitionDirection ? 1 : -1;

    if (transitionDirection && transitionOffset > width || !transitionDirection && transitionOffset < -width) {
        isTransitioning = false;
        isWatchFaceChangeAllowed = true;
        transitionOffset = 0;
        currentWatchFace = nextFaceIndex;
    }
}

void WatchFaceManager::performBrightnessTransition() {
    unsigned long currentTime = millis();

    if (currentTime - lastBrightnessChange > 20) {
        lastBrightnessChange = currentTime;

        targetBrigthness > currentBrightness ? currentBrightness++ : currentBrightness--;
        if (currentBrightness == targetBrigthness) isBrightnessTransitioning = false;
        FastLED.setBrightness(currentBrightness);
    }
}