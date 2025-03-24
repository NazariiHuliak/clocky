#include "WatchFaceManager.h"
#include <utils/matrix/MatrixUtil.h>

#include "data/network/NetworkDataManager.h"

WatchFaceManager::WatchFaceManager(WatchFace **watchFaces,
                                   const uint8_t count) : watchFaces(watchFaces), m_count(count) {
}

WatchFaceManager::~WatchFaceManager() {
    for (uint8_t i = 0; i < m_count; i++) {
        delete watchFaces[i];
    }
    delete[] watchFaces;
}

void WatchFaceManager::begin() {
    xTaskCreatePinnedToCore(
        WatchFaceManager::updateWrapper,
        "updateTask",
        16384,
        this,
        1,
        NULL,
        0
    );
}

bool WatchFaceManager::getIsWatchFaceChangeAllowed() const {
    return isWatchFaceChangeAllowed && !isTransitioning;
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

void WatchFaceManager::updateWrapper(void *parameter) {
    WatchFaceManager *obj = static_cast<WatchFaceManager *>(parameter);
    if (obj == nullptr) {
        vTaskDelete(NULL);
        return;
    }

    while (true) {
        obj->update();
    }
}

bool WatchFaceManager::isUpdateDataAllowed() const {
    for (uint8_t i = 0; i < m_count; i++) {
        if (!watchFaces[i]->isExternalUpdateAllowed()) {
            Log::info(("WatchFace [" + String(i) + "]. Does not allow external update").c_str());
            return false;
        }
    }

    return true;
}

void WatchFaceManager::update() {
    const unsigned long currentTime = millis();
    checkAirAlert(currentTime);
    updateWatchFacesData(currentTime);
}

void WatchFaceManager::checkAirAlert(unsigned long updateTime) {
    if (updateTime - lastAirAlertCheck <= AIR_ALERT_UPDATE_PERIOD) return;
    lastAirAlertCheck = updateTime;

    bool isAirAlert = NetworkDataManager::instance().getAirAlert().alertActive;
    if (isAirAlert && !wasTransitionedToAirAlert) {
        initiateTransition(airAlertWatchFace);
        wasTransitionedToAirAlert = true;
    } else if (!isAirAlert && wasTransitionedToAirAlert) {
        wasTransitionedToAirAlert = false;
    }
}

void WatchFaceManager::updateWatchFacesData(unsigned long updateTime) {
    if (updateTime - lastTimeDataUpdate >= checkUpdatePeriod) {
        lastTimeDataUpdate = updateTime;

        const bool globalUpdateAllowed = isUpdateDataAllowed();
        for (uint8_t i = 0; i < m_count; i++) {
            if (i != currentWatchFace) {
                watchFaces[i]->resetMode();
                if (!globalUpdateAllowed) continue;
            } else {
                isWatchFaceChangeAllowed = watchFaces[i]->isWatchFaceChangeAllowed();
            }

            const unsigned long lastTimeUpdate = watchFaces[i]->getLastTimeDataUpdate();
            if (updateTime - lastTimeUpdate >= watchFaces[i]->getUpdateDataPeriod() || lastTimeUpdate == 0) {
                watchFaces[i]->updateData(updateTime);
            }
        }
    }
}

void WatchFaceManager::initiateTransition(const uint8_t nextValue_) {
    if (currentWatchFace == nextValue_) return;
    Log::info(("Transition initiated to: " + String(nextValue)).c_str());

    isTransitioning = true;
    transitionOffset = 0;
    nextValue = nextValue_;
    transitionDirection = currentWatchFace > nextValue_;

    isWatchFaceChangeAllowed = false;
}

void WatchFaceManager::initiateTransition(const bool direction) {
    Log::info(("Transition initiated with direction: " + String(direction)).c_str());

    isTransitioning = true;
    transitionOffset = 0;
    nextValue = (currentWatchFace + (direction ? 1 : m_count - 1)) % m_count;
    transitionDirection = direction;

    isWatchFaceChangeAllowed = false;
}

void WatchFaceManager::performTransition() {
    FastLED.clear();

    watchFaces[currentWatchFace]->showFrame(-transitionOffset);
    watchFaces[nextValue]->showFrame(transitionDirection
                                         ? MATRIX_WIDTH - transitionOffset
                                         : -MATRIX_WIDTH - transitionOffset);

    transitionOffset += transitionDirection ? 1 : -1;

    if (transitionDirection && transitionOffset > MATRIX_WIDTH ||
        !transitionDirection && transitionOffset < -MATRIX_WIDTH) {
        isTransitioning = false;
        transitionOffset = 0;

        isWatchFaceChangeAllowed = true;
        currentWatchFace = nextValue;
    }
}
