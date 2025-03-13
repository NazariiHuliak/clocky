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
public:
    static NetworkDataManager &instance() {
        static NetworkDataManager instance;
        return instance;
    }

    NetworkDataManager(const NetworkDataManager &) = delete;

    NetworkDataManager &operator=(const NetworkDataManager &) = delete;

    void update(unsigned long updateTime) {
        alertCache.update(updateTime);
        weatherCache.update(updateTime);
        currencyExchangeCache.update(updateTime);
    }

    AirAlert getAirAlert() {
        return alertCache.get();
    }

    Weather getWeather() {
        return weatherCache.data;
    }

    CurrencyExchange getCurrencyExchange() {
        return currencyExchangeCache.data;
    }

private:
    NetworkDataManager() {
    }

    AirAlertCache alertCache;
    WeatherCache weatherCache;
    CurrencyExchangeCache currencyExchangeCache;
};


#endif
