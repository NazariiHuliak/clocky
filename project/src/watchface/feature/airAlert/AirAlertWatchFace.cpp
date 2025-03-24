#include "AirAlertWatchFace.h"

#include "data/network/NetworkDataManager.h"
#include "resources/icon/signs.h"

AirAlertWatchFace::AirAlertWatchFace(CRGB *leds): WatchFace(leds) {
}

bool AirAlertWatchFace::isWatchFaceChangeAllowed() {
    return true;
}

unsigned long AirAlertWatchFace::getUpdateDataPeriod() {
    return updateDataPeriod;
}

unsigned long AirAlertWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void AirAlertWatchFace::showFrame(const int16_t xOffset) {
    bool isAirAlert = airAlert.alertActive;

    drawer.setSentence(Position2D(8 + xOffset, 1),
                       isAirAlert ? "ALRT" : "NONE");

    drawer.setIcon(
        Position2D(xOffset, 0),
        isAirAlert ? airAlertSign : noAirAlertSign,
        false);
}

void AirAlertWatchFace::nextMode() {
}

void AirAlertWatchFace::resetMode() {
    currentModeIndex = 0;
}

bool AirAlertWatchFace::isExternalUpdateAllowed() {
    return true;
}

void AirAlertWatchFace::updateData(unsigned long updateTime) {
    if (updateTime - lastDataUpdate < updateDataPeriod) return;

    lastDataUpdate = updateTime;
    airAlert = NetworkDataManager::instance().getAirAlert();
    Log::info("AirAlertWatchFace: was synchronized at ", String(updateTime), ".");
}
