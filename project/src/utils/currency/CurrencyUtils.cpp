#include "CurrencyUtils.h"

Currency CurrencyUtils::findMostFrequentCurrency(
    const std::vector<Pair<Currency, Currency>> &currencyPairs,
    const std::set<int> &excludedIndexes) {
    std::map<Currency, int> currencyCount;

    for (size_t i = 0; i < currencyPairs.size(); i++) {
        if (excludedIndexes.count(i)) continue;  // Skip already processed indexes

        const auto& pair = currencyPairs[i];
        currencyCount[pair.key]++;
        currencyCount[pair.value]++;
    }

    Currency mostFrequent = Currency::Unknown;
    int maxCount = 0;

    for (const auto& entry : currencyCount) {
        if (entry.second > maxCount) {
            mostFrequent = entry.first;
            maxCount = entry.second;
        }
    }

    return mostFrequent;
}

std::vector<Pair<Currency, bool>> CurrencyUtils::buildReversedSignMap(const std::vector<Pair<Currency, Currency>> &currencyPairs,
                                                  Currency targetCurrency, std::set<int> &processedIndexes){
    std::vector<Pair<Currency, bool>> result;

    for (size_t i = 0; i < currencyPairs.size(); i++) {
        if (processedIndexes.count(i)) continue;

        const auto& pair = currencyPairs[i];

        if (pair.key == targetCurrency) {
            result.emplace_back(pair.value, false);
            processedIndexes.insert(i);
        } else if (pair.value == targetCurrency) {
            result.emplace_back(pair.key, true);
            processedIndexes.insert(i);
        }
    }

    return result;
}

std::vector<Pair<Currency, std::vector<Pair<Currency, bool>>>> CurrencyUtils::processCurrencyPairs(
    const std::vector<Pair<Currency, Currency>> &currencyPairs
) {
    std::vector<Pair<Currency, std::vector<Pair<Currency, bool>>>> result;
    std::set<int> processedIndexes;

    while (processedIndexes.size() < currencyPairs.size()) {
        Currency mostFrequent = findMostFrequentCurrency(currencyPairs, processedIndexes);
        std::vector<Pair<Currency, bool>> adjustedPairs = buildReversedSignMap(currencyPairs, mostFrequent, processedIndexes);

        result.push_back(Pair<Currency, std::vector<Pair<Currency, bool>>>(mostFrequent, adjustedPairs));
    }

    return result;
}

const char* CurrencyUtils::toString(Currency currency) {
    switch (currency) {
        case Currency::UAH: return "UAH";
        case Currency::USD: return "USD";
        case Currency::EUR: return "EUR";
        case Currency::PLN: return "PLN";
        case Currency::KRN: return "KRN";
        case Currency::JPY: return "JPY";
        case Currency::GBP: return "GBP";
        case Currency::AUD: return "AUD";
        default: return "Unknown";
    }
}

Currency CurrencyUtils::toEnum(const String &currencyStr) {
    if (currencyStr == "UAH") return Currency::UAH;
    if (currencyStr == "USD") return Currency::USD;
    if (currencyStr == "EUR") return Currency::EUR;
    if (currencyStr == "PLN") return Currency::PLN;
    if (currencyStr == "KRN") return Currency::KRN;
    if (currencyStr == "JPY") return Currency::JPY;
    if (currencyStr == "GBP") return Currency::GBP;
    if (currencyStr == "AUD") return Currency::AUD;

    return Currency::Unknown;
}