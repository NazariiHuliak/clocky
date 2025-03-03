#include "WatchFaceManager.h"
#include <utils/matrix/MatrixUtil.h>

WatchFaceManager::WatchFaceManager(WatchFace** watchFaces, const uint8_t count) : watchFaces(watchFaces), m_count(count) {}

WatchFaceManager::~WatchFaceManager() {
    for (uint8_t i = 0; i < m_count; i++) {
        delete watchFaces[i];
    }
    delete[] watchFaces;
}

bool WatchFaceManager::getIsWatchFaceChangeAllowed() const {
    return isWatchFaceChangeAllowed && !isTransitioning;
}

void WatchFaceManager::update() {
    updateWatchFacesData();
}

void WatchFaceManager::updateWatchFacesData() {
    const unsigned long currentTime = millis();

    if (currentTime - lastTimeDataUpdate >= checkUpdatePeriod) {
        lastTimeDataUpdate = currentTime;

        const bool globalUpdateAllowed = isUpdateDataAllowed();
        for (uint8_t i = 0; i < m_count; i++) {
            if (i != currentWatchFace) {
                watchFaces[i]->resetMode();
                if (!globalUpdateAllowed) continue;
            } else {
                isWatchFaceChangeAllowed = watchFaces[i]->isWatchFaceChangeAllowed();
            }

            const unsigned long lastTimeUpdate = watchFaces[i]->getLastTimeDataUpdate();
            if (currentTime - lastTimeUpdate >= watchFaces[i]->getUpdateDataPeriod() || lastTimeUpdate == 0) {
                watchFaces[i]->updateData(currentTime);
            }
        }
    }
}

void WatchFaceManager::showWatchFace() {
    if (isTransitioning) {
        performTransition();
    } else if (watchFaces[currentWatchFace]) {
        watchFaces[currentWatchFace]->showFrame(0);
    }
}

void WatchFaceManager::previousWatchFace() {
    if (!isWatchFaceChangeAllowed) return;

    initiateTransition(false);
}

void WatchFaceManager::nextMode() const {
    watchFaces[currentWatchFace]->nextMode();
}

void WatchFaceManager::nextWatchFace() {
    if (!isWatchFaceChangeAllowed) return;

    initiateTransition(true);
}

void WatchFaceManager::resetCurrentWatchFace() const {
    watchFaces[currentWatchFace]->resetMode();
}

bool WatchFaceManager::isUpdateDataAllowed() const {
    for (uint8_t i = 0; i < m_count; i++) {
        if (!watchFaces[i]->isExternalUpdateAllowed()) return false;
    }

    return true;
}

void WatchFaceManager::initiateTransition(const bool direction) {
    isTransitioning = true;
    transitionOffset = 0;
    nextValue = (currentWatchFace + (direction ? 1 : m_count - 1)) % m_count;
    transitionDirection = direction;

    isWatchFaceChangeAllowed = false;
}

void WatchFaceManager::performTransition() {
    FastLED.clear();

    watchFaces[currentWatchFace]->showFrame(-transitionOffset);
    watchFaces[nextValue]->showFrame(transitionDirection ? width - transitionOffset : -width - transitionOffset);

    transitionOffset += transitionDirection ? 1 : -1;

    if (transitionDirection && transitionOffset > width || !transitionDirection && transitionOffset < -width) {
        isTransitioning = false;
        transitionOffset = 0;

        isWatchFaceChangeAllowed = true;
        currentWatchFace = nextValue;
    }
}