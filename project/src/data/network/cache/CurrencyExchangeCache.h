#ifndef CURRENCYEXCHANGECACHE_H
#define CURRENCYEXCHANGECACHE_H

#include <vector>

#include "../src/resources/Config.h"
#include <../src/core/state/Updatable.h>
#include "../src/core/model/network/CurrencyExchange.h"
#include "../src/core/state/Stateful.h"
#include "utils/currency/CurrencyUtils.h"
#include "utils/json/JsonUtils.h"

class CurrencyExchangeCache : public Updatable {
    Stateful<CurrencyExchange> statefulData;
    std::vector<Pair<Currency, std::vector<Pair<Currency, bool> > > > parseParams;
    uint8_t numRequests;
    uint8_t currentRequest = 0;

public:
    CurrencyExchangeCache() {
        parseParams = CurrencyUtils::processCurrencyPairs(CURRENCY_EXCHANGE_PAIRS);
        numRequests = parseParams.size();
        for (const auto &entry: parseParams) {
            Serial.print("Most Frequent: ");
            Serial.println(CurrencyUtils::toString(entry.key));

            for (const auto &p: entry.value) {
                Serial.print(p.toString());
                Serial.print(" ");
            }
            Serial.println();
        }
    };

    ~CurrencyExchangeCache() override = default;

    CurrencyExchange get() {
        return statefulData.data;
    }

    void update(unsigned long updateTime) override {
        if (!canBeUpdateAt(updateTime)) return;
        if (currentRequest == numRequests) {
            currentRequest = 0;
            statefulData.data.numCurrencies = 0;
        }
        statefulData.isUpdating = true;

        Log::info("Currency exchange: request ", String(currentRequest + 1), "/", String(numRequests),
                  ". Fetch at: ", String(updateTime));

        String endPoint = String(CURRENCY_EXCHANGE_ENDPOINT) + "/" +
                          String(CurrencyUtils::toString(parseParams[currentRequest].key));
        sendGetRequest(CURRENCY_EXCHANGE_HOST,CURRENCY_EXCHANGE_PORT, endPoint,
                       &statefulData, parseCurrencyExchange, &parseParams[currentRequest]);
        currentRequest++;
    }

    bool isUpdating() override {
        return statefulData.isUpdating;
    }

    bool canBeUpdateAt(unsigned long updateTime) override {
        bool timeRequirement = statefulData.data.lastUpdate == 0 ||
                               (currentRequest == numRequests
                                    ? (updateTime - statefulData.data.lastUpdate >= CURRENCY_EXCHANGE_UPDATE_PERIOD)
                                    : (updateTime - statefulData.data.lastUpdate >=
                                       CURRENCY_EXCHANGE_FREQUENCY_REQUEST_TIME));

        return !statefulData.isUpdating &&
               timeRequirement &&
               getPermits() > 0;
    }
};

#endif
