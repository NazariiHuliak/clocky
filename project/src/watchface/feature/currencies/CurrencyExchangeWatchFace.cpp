#include "CurrencyExchangeWatchFace.h"

#include "data/network/NetworkDataManager.h"
#include "utils/log/Log.h"

CurrencyExchangeWatchFace::CurrencyExchangeWatchFace(CRGB *leds) : WatchFace(leds) {}

bool CurrencyExchangeWatchFace::isWatchFaceChangeAllowed() {
    return false;
}

unsigned long CurrencyExchangeWatchFace::getUpdateDataPeriod() {
    return 0;
}

unsigned long CurrencyExchangeWatchFace::getLastTimeDataUpdate() {
    return 0;
}

void CurrencyExchangeWatchFace::showFrame(int16_t xOffset) {}

void CurrencyExchangeWatchFace::nextMode() {}

void CurrencyExchangeWatchFace::resetMode() {}

bool CurrencyExchangeWatchFace::isExternalUpdateAllowed() {
    return false;
}

void CurrencyExchangeWatchFace::updateData(unsigned long updateTime) {
    if (updateTime - lastDataUpdate < updateDataPeriod) return;

    lastDataUpdate = updateTime;
    currencyExchange = NetworkDataManager::instance().getCurrencyExchange();
    Log::info("CurrencyExchangeWatchFace: was synchronized at ", String(updateTime), ".");
}