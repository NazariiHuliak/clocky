#ifndef CURRENCYEXCHANGEWATCHFACE_H
#define CURRENCYEXCHANGEWATCHFACE_H

#include "core/model/network/CurrencyExchange.h"
#include "watchface/core/WatchFace.h"

class CurrencyExchangeWatchFace : public WatchFace {
    // Modes
    static constexpr uint8_t numModes = 4;
    static constexpr uint8_t mainMode = 0;

    // Data
    CurrencyExchange currencyExchange;

    // Update
    const unsigned long updateDataPeriod = 30000;
    unsigned long lastDataUpdate = 0;
public:
    explicit CurrencyExchangeWatchFace(CRGB *leds);
    ~CurrencyExchangeWatchFace() override = default;

    bool isWatchFaceChangeAllowed() override;
    unsigned long getUpdateDataPeriod() override;
    unsigned long getLastTimeDataUpdate() override;

    void showFrame(int16_t xOffset) override;
    void nextMode() override;
    void resetMode() override;
    bool isExternalUpdateAllowed() override;
    void updateData(unsigned long updateTime) override;
};

#endif
