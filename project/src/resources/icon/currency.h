#ifndef CURRENCYICON_H
#define CURRENCYICON_H

#include <Arduino.h>
#include <unordered_map>
#include <../src/resources/Config.h>

const uint8_t USD_[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 10, 10, 0, 0},
    {0, 0, 10, 3, 0, 0, 0},
    {0, 0, 0, 10, 0, 0, 0},
    {0, 0, 0, 3, 10, 0, 0},
    {0, 0, 0, 3, 10, 0, 0},
    {0, 0, 10, 10, 0, 0, 0},
    {0, 0, 0, 3, 0, 0, 0}};

const uint8_t EUR_[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 0, 0, 0, 0, 0},
    {0, 0, 4, 4, 4, 0, 0},
    {0, 4, 0, 0, 0, 0, 0},
    {0, 4, 4, 4, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0},
    {0, 4, 4, 4, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0},
    {0, 0, 4, 4, 4, 0, 0}};

const uint8_t UAH_[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 0, 0, 0, 0, 0},
    {0, 4, 4, 4, 0, 0, 0},
    {0, 0, 0, 0, 4, 0, 0},
    {0, 3, 3, 4, 0, 0, 0},
    {0, 0, 4, 0, 0, 0, 0},
    {0, 4, 3, 3, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0},
    {0, 0, 4, 4, 4, 0, 0}};

const uint8_t PLN_[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 0, 0, 0, 0, 0},
    {0, 4, 4, 4, 0, 4, 0},
    {0, 0, 0, 4, 0, 4, 4},
    {0, 0, 4, 0, 4, 4, 0},
    {0, 4, 0, 0, 0, 4, 0},
    {0, 4, 0, 0, 0, 4, 0},
    {0, 4, 4, 4, 0, 4, 0},
    {0, 0, 0, 0, 0, 0, 0}};


const std::unordered_map<Currency, const uint8_t (*)[ICON_HEIGHT][ICON_WIDTH]> currencyToIcon = {
    {USD, &USD_},
    {EUR, &EUR_},
    {UAH, &UAH_},
    {PLN, &PLN_}
};

const uint8_t (*getCurrencyIcon(const Currency& currency))[ICON_HEIGHT][ICON_WIDTH] {
    auto it = currencyToIcon.find(currency);
    if (it != currencyToIcon.end()) {
        return it->second;
    }

    // TODO: Return a pointer to an "unknown" icon
    return nullptr;
}


#endif
