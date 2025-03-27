#ifndef NETWORKDATAMANAGER_H
#define NETWORKDATAMANAGER_H

#include <Arduino.h>

#include <../src/core/model/network/AirAlert.h>
#include <../src/core/model/network/Weather.h>
#include <../src/core/model/network/CurrencyExchange.h>

#include <../src/data/network/cache/AirAlertCache.h>
#include <../src/data/network/cache/WeatherCache.h>
#include <../src/data/network/cache/CurrencyExchangeCache.h>


/**
 * @brief Manages network data caches for various data models.
 *
 * The NetworkDataManager class is a singleton that provides a unified
 * interface for updating and retrieving network data. It encapsulates
 * several cache models including AirAlertCache, WeatherCache, and
 * CurrencyExchangeCache, ensuring that only one instance exists and
 * providing a single point of access throughout the application.
 */
class NetworkDataManager {
    AirAlertCache alertCache;
    CurrencyExchangeCache currencyExchangeCache;

    NetworkDataManager() {
    }

    ~NetworkDataManager() = default;

    bool canUpdateNext() {
        // TODO: think how to rewrite it
        // Log::info("AirAlert update: ", String(alertCache.isUpdating()));
        // Log::info("CurrencyExchange update:", String(currencyExchangeCache.isUpdating()));
        return !alertCache.isUpdating() && !currencyExchangeCache.isUpdating();
    }

public:
    static NetworkDataManager &instance() {
        static NetworkDataManager instance;
        return instance;
    }

    NetworkDataManager(const NetworkDataManager &) = delete;

    NetworkDataManager &operator=(const NetworkDataManager &) = delete;

    AirAlert getAirAlert() {
        return alertCache.get();
    }

    CurrencyExchange getCurrencyExchange() {
        return currencyExchangeCache.get();
    }

    void updateEmergencyData() {
        alertCache.update(millis());
    }

    void updateAll() {
        // if (canUpdateNext()) alertCache.update(millis());
        if (canUpdateNext()) currencyExchangeCache.update(millis());
    }
};


#endif
