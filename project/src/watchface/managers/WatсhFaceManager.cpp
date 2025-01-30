#include "WatchFaceManager.h"

WatchFaceManager::WatchFaceManager(WatchFace** watchFaces, uint8_t count) : watchfaces(watchFaces), m_count(count) {}

WatchFaceManager::~WatchFaceManager() {
    for (uint8_t i = 0; i < m_count; i++) {
        delete watchfaces[i];
    }
    delete[] watchfaces;
}

bool WatchFaceManager::getIsWatchFaceChangeAllowed() {
    return isWatchFaceChangeAllowed && !isTransitioning;
}

void WatchFaceManager::updateWatchFaceData() {
    unsigned long currentTime = millis();

    if (currentTime - lastTimeDataUpdate >= checkUpdatePeriod) {
        lastTimeDataUpdate = currentTime;

        for (uint8_t i = 0; i < m_count; i++) {
            if (currentTime - watchfaces[i]->getLastTimeDataUpdate() >= watchfaces[i]->getUpdateDataPeriod()) {
                watchfaces[i]->updateData();
                watchfaces[i]->setLastTimeDataUpdate(currentTime);
            }
            if (currentWatchFace != i) {
                watchfaces[i]->resetMode();
            }
        }
    }
}

void WatchFaceManager::showWatchFace() {
    if (isTransitioning) {
        performSlideTransition();
    } else if (watchfaces[currentWatchFace]) {
        watchfaces[currentWatchFace]->showFrame();
    }
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