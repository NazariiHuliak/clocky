#ifndef CURRENCYEXCHANGEWATCHFACE_H
#define CURRENCYEXCHANGEWATCHFACE_H

#include "core/model/network/CurrencyExchange.h"
#include "core/transition/MultiInitiationTransitionable.h"
#include "core/transition/Transitionable.h"
#include "data/buttons/ButtonHandler.h"
#include "watchface/core/WatchFace.h"


class CurrencyExchangeWatchFace : public WatchFace, public MultiInitiationTransitionable {
    /** Modes <br>
     * 0) Show first currencies signs (allowed change watchface) <br>
     * 1) Show current currencies signs (not allowed to change watchface) <br>
     * 2) Show current currency rate (not allowed to change watchface) */
    static constexpr uint8_t numModes = 3;
    static constexpr uint8_t mainMode = 0;
    uint8_t currentModeIndex = 0;

    // Data
    ButtonHandler &buttonHandler;
    uint8_t currencyExchangeIndex = 0;
    CurrencyExchange currencyExchange;

    // Update intervals
    const unsigned long updateDataPeriod = 30000;
    unsigned long lastDataUpdate = 0;
    const unsigned long buttonsCheckPeriod = 10;
    unsigned long lastButtonsCheck = 0;

    // Animation
    bool transitionType = false;
    void initiateTransition(uint8_t nextValue_) override;
    void initiateTransition(bool direction) override;
    void performTransition() override;
    void performCurrencyPairTransition();
    void performModeTransition();

    void showFrame(uint8_t mode, uint8_t currentPair, int16_t xOffset, int16_t yOffset);
    void showCurrencySigns(uint8_t currentPair, int16_t xOffset, int16_t yOffset);
    void showCurrencyRate(uint8_t currentPair, int16_t xOffset, int16_t yOffset);

    void getDigits(double rate, int8_t &dotSlot, int8_t (&digits)[6]);
    void checkButtons();
public:
    explicit CurrencyExchangeWatchFace(CRGB *leds, ButtonHandler &buttons);
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
