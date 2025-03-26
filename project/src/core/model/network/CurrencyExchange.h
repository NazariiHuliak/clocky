#ifndef CURRENCYEXCHANGEDATA_H
#define CURRENCYEXCHANGEDATA_H

#include "Arduino.h"
#include "resources/Currency.h"

struct ExchangeRate {
    Currency fromCurrency;
    Currency toCurrency;
    double rate;
};

struct CurrencyExchange {
    ExchangeRate exchangeRate[5];
    uint8_t numCurrencies;
    unsigned long lastUpdate;
};

#endif
