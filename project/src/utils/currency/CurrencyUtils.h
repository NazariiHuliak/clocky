#ifndef CURRENCYUTILS_H
#define CURRENCYUTILS_H

#include <vector>
#include <map>
#include <set>
#include <Arduino.h>

#include "core/model/Pair.h"
#include "resources/Currency.h"

class CurrencyUtils {
public:
    static std::vector<Pair<Currency, std::vector<Pair<Currency, bool>>>> processCurrencyPairs(
        const std::vector<Pair<Currency, Currency>> &currencyPairs);

    static const char *toString(Currency currency);
    static Currency toEnum(const String &currencyStr);
private:
    static Currency findMostFrequentCurrency(const std::vector<Pair<Currency, Currency> > &currencyPairs,
                                             const std::set<int> &processedIndexes);

    static std::vector<Pair<Currency, bool> > buildReversedSignMap(
        const std::vector<Pair<Currency, Currency> > &currencyPairs,
        Currency targetCurrency, std::set<int> &processedIndexes);
};

#endif
