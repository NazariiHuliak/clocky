#include "CurrencyExchangeWatchFace.h"

#include "data/network/NetworkDataManager.h"
#include "utils/log/Log.h"
#include "../src/resources/icon/currency.h"
#include "../src/resources/icon/signs.h"
#include "data/buttons/ButtonHandler.h"
#include "resources/icon/currency.h"

CurrencyExchangeWatchFace::CurrencyExchangeWatchFace(CRGB *leds, ButtonHandler &buttons) : WatchFace(leds),
    buttonHandler(buttons) {
}

bool CurrencyExchangeWatchFace::isWatchFaceChangeAllowed() {
    return currentModeIndex == 0;
}

unsigned long CurrencyExchangeWatchFace::getUpdateDataPeriod() {
    return (currentModeIndex == 0) ? updateDataPeriod : buttonsCheckPeriod;
}

unsigned long CurrencyExchangeWatchFace::getLastTimeDataUpdate() {
    return lastDataUpdate;
}

void CurrencyExchangeWatchFace::showFrame(int16_t xOffset) {
    if (currencyExchange.lastUpdate == 0) {
        drawer.setIcon(Position2D(xOffset, 0), EUR_, false);
        drawer.setIcon(Position2D(17 + xOffset, 0), USD_, false);

        if (isWiFiConnected()) {
            drawer.setIcon(Position2D(9 + xOffset, -1), noData, false);
        } else {
            drawer.setIcon(Position2D(9 + xOffset, 1), noWifi, false);
        }

        return;
    }

    isTransitioning
        ? performTransition()
        : showFrame(currentModeIndex, currencyExchangeIndex, xOffset, 0);
}

void CurrencyExchangeWatchFace::showFrame(uint8_t mode, uint8_t currentPair, int16_t xOffset, int16_t yOffset) {
    if (mode == 0 || mode == 1) {
        showCurrencySigns(currentPair, xOffset, yOffset);
    } else if (mode == 2) {
        showCurrencyRate(currentPair, xOffset, yOffset);
    }
}

void CurrencyExchangeWatchFace::nextMode() {
    uint8_t nextModeIndex = (currentModeIndex == 0) ? 2 : currentModeIndex % (numModes - 1) + 1;
    initiateTransition(nextModeIndex);
}

void CurrencyExchangeWatchFace::resetMode() {
    currentModeIndex = 0;
}

bool CurrencyExchangeWatchFace::isExternalUpdateAllowed() {
    return true;
}

void CurrencyExchangeWatchFace::updateData(unsigned long updateTime) {
    if (currentModeIndex != 0) {
        checkButtons();
    }

    bool isFirstUpdate = lastDataUpdate == 0 && updateTime - lastDataUpdate >= 2000;
    if (!isFirstUpdate && updateTime - lastDataUpdate < updateDataPeriod) return;
    lastDataUpdate = updateTime;
    currencyExchange = NetworkDataManager::instance().getCurrencyExchange();
    Log::info("CurrencyExchangeWatchFace: was synchronized at ", String(updateTime));
}

void CurrencyExchangeWatchFace::initiateTransition(uint8_t nextMode) {
    // Transition for mode changing

    nextValue = nextMode;
    isTransitioning = true;
    transitionType = false;
    transitionOffset = 0;
}

void CurrencyExchangeWatchFace::initiateTransition(bool direction) {
    // Transition for currency changing. nextValue stores next currency index

    nextValue = (currencyExchangeIndex + 1) % currencyExchange.numCurrencies;
    isTransitioning = true;
    transitionType = true;
    transitionDirection = direction;
    transitionOffset = 0;
}

void CurrencyExchangeWatchFace::performTransition() {
    FastLED.clear();

    transitionType
        ? performCurrencyPairTransition()
        : performModeTransition();
}

void CurrencyExchangeWatchFace::performCurrencyPairTransition() {
    showFrame(currentModeIndex, currencyExchangeIndex, -transitionOffset, 0);
    showFrame(0, nextValue, transitionDirection
                                ? MATRIX_WIDTH - transitionOffset
                                : -MATRIX_WIDTH - transitionOffset, 0);

    transitionOffset += transitionDirection ? 1 : -1;

    if (transitionDirection && transitionOffset > MATRIX_WIDTH ||
        !transitionDirection && transitionOffset < -MATRIX_WIDTH) {
        isTransitioning = false;
        transitionOffset = 0;
        currencyExchangeIndex = nextValue;
        currentModeIndex = 1; // Currencies sign (not allowed to change watchFace)
    }
}

void CurrencyExchangeWatchFace::performModeTransition() {
    Log::info("performModeTransition");
    showFrame(currentModeIndex, currencyExchangeIndex, 0, -transitionOffset);
    showFrame(nextValue, currencyExchangeIndex, 0, -MATRIX_HEIGHT - transitionOffset);

    transitionOffset -= 1;

    if (transitionOffset < -MATRIX_HEIGHT) {
        isTransitioning = false;
        transitionOffset = 0;
        currentModeIndex = nextValue;
    }
}

void CurrencyExchangeWatchFace::showCurrencySigns(uint8_t currentPair, int16_t xOffset, int16_t yOffset) {
    const uint8_t (*fromCurrencyIcon)[ICON_HEIGHT][ICON_WIDTH] = getCurrencyIcon(
        currencyExchange.exchangeRate[currentPair].fromCurrency);
    const uint8_t (*toCurrencyIcon)[ICON_HEIGHT][ICON_WIDTH] = getCurrencyIcon(
        currencyExchange.exchangeRate[currentPair].toCurrency);

    bool showAbbreviation = fromCurrencyIcon == nullptr || toCurrencyIcon == nullptr;

    if (showAbbreviation) {
        drawer.setAbbreviation(
            Position2D(xOffset, 1 + yOffset),
            CurrencyUtils::toString(currencyExchange.exchangeRate[currentPair].fromCurrency));
        drawer.setPixel(Position2D(11 + xOffset, 3 + yOffset), CHSV{95, 255, 255});
        drawer.setPixel(Position2D(12 + xOffset, 3 + yOffset), CHSV{95, 255, 255});
        drawer.setAbbreviation(
            Position2D(13 + xOffset, 1 + yOffset),
            CurrencyUtils::toString(currencyExchange.exchangeRate[currentPair].toCurrency));
    } else {
        drawer.setIcon(
            Position2D(1 + xOffset, yOffset),
            *fromCurrencyIcon,
            false);
        drawer.setIcon(
            Position2D(8 + xOffset, yOffset - 1),
            rightArrow,
            false);
        drawer.setIcon(
            Position2D(16 + xOffset, yOffset),
            *toCurrencyIcon,
            false);
    }
}

void CurrencyExchangeWatchFace::showCurrencyRate(uint8_t currentPair, int16_t xOffset, int16_t yOffset) {
    double rate = currencyExchange.exchangeRate[currentPair].rate;
    int8_t dotSlot;
    int8_t digits[6];
    getDigits(rate, dotSlot, digits);

    uint8_t digitWidth = 4;
    bool isBigNumber = digits[5] != -1;
    uint8_t totalDigits = isBigNumber ? 6 : 4;
    for (uint8_t i = 0; i < totalDigits; i++) {
        if (digits[i] != -1) {
            uint8_t offset = (dotSlot != -1 && i >= dotSlot) ? 2 : 0; // 2 is dot shift
            uint8_t digitX = xOffset + i * digitWidth + offset;
            drawer.setDigit(Position2D(digitX, 2 + yOffset), digits[i]);
        }
    }

    if (dotSlot != -1) {
        int8_t dotX = xOffset + dotSlot * digitWidth;
        drawer.setPixel(Position2D(dotX, 6 + yOffset));
    }

    if (!isBigNumber) {
        const uint8_t (*toCurrencyIcon)[ICON_HEIGHT][ICON_WIDTH] = getCurrencyIcon(
            currencyExchange.exchangeRate[currentPair].toCurrency);

        if (toCurrencyIcon != nullptr) {
            drawer.setIcon(
                Position2D(17 + xOffset, yOffset),
                *getCurrencyIcon(currencyExchange.exchangeRate[currentPair].toCurrency),
                false);
        } else {
            drawer.setIcon(
                Position2D(17 + xOffset, yOffset),
                coin,
                false);
        }
    }
}

void CurrencyExchangeWatchFace::getDigits(double rate, int8_t &dotSlot, int8_t (&digits)[6]) {
    uint32_t integerPart = static_cast<int>(rate);
    uint8_t beforeCount = (integerPart == 0) ? 1 : static_cast<uint8_t>(log10(integerPart) + 1);
    dotSlot = beforeCount;

    uint8_t totalDigits = (beforeCount > 4) ? 6 : 4;
    uint32_t scale = pow(10, totalDigits - beforeCount);
    uint32_t scaledRate = static_cast<int>(rate * scale);

    for (uint8_t i = 0; i < 6; i++) {
        if (i >= totalDigits) {
            digits[i] = -1;
        } else {
            uint32_t divisor = pow(10, totalDigits - 1 - i);
            digits[i] = (scaledRate / divisor) % 10;
        }
    }

    if (dotSlot == totalDigits) {
        dotSlot = -1;
        return;
    }

    while (dotSlot != -1 && digits[totalDigits - 1] == 0) {
        if (dotSlot < totalDigits - 1) {
            for (int i = totalDigits - 1; i > 0; i--) {
                digits[i] = digits[i - 1];
            }
            digits[0] = -1;
            dotSlot++;
        } else {
            dotSlot = -1;
        }
    }
}

void CurrencyExchangeWatchFace::checkButtons() {
    switch (buttonHandler.processButtons()) {
        case 1:
            initiateTransition(false);
            break;
        case 2:
            nextMode();
            break;
        case 3:
            initiateTransition(true);
            break;
        case 5:
            initiateTransition((uint8_t) 0);
            break;
        default:
            break;
    }
}
