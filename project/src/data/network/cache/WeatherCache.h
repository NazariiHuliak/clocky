#ifndef WEATHERCACHE_H
#define WEATHERCACHE_H

#include <Arduino.h>
#include "../src/resources/Config.h"
#include "../src/core/model/network/Weather.h"
#include <../src/utils/json/JsonUtils.h>
#include <../src/utils/network/NetworkUtils.h>
#include <../src/core/state/Updatable.h>

class WeatherCache : public Updatable {
public:
    static Weather data;

    static WeatherCache& instance() {
        static WeatherCache inst;
        return inst;
    }

    void update(unsigned long updateTime) override {
        //TODO: implement this method
    }
};

#endif
