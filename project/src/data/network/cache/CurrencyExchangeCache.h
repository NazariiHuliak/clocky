#ifndef CURRENCYEXCHANGECACHE_H
#define CURRENCYEXCHANGECACHE_H

#include "../src/resources/Config.h"
#include <../src/core/state/Updatable.h>
#include "../src/core/model/network/CurrencyExchange.h"

class CurrencyExchangeCache: public Updatable {
public:
    static CurrencyExchange data;

    static CurrencyExchangeCache& instance() {
          static CurrencyExchangeCache inst;
          return inst;
    }

    void update(unsigned long updateTime) override {
        //TODO: implement this method
    }
};

#endif
