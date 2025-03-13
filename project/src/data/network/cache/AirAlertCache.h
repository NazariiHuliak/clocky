#ifndef AIRALERTCACHE_H
#define AIRALERTCACHE_H

#include <Arduino.h>
#include "../src/resources/Config.h"
#include "../src/utils/log/Log.h"
#include "../src/core/model/network/AirAlert.h"
#include <../src/utils/network/JsonUtils.h>
#include <../src/utils/network/HttpUtils.h>
#include <../src/core/update/Updatable.h>

class AirAlertCache : public Updatable {
    AirAlert data;

public:
    AirAlertCache() {
    };

    ~AirAlertCache() override = default;

    AirAlert get() {
        return data;
    }

    void update(unsigned long updateTime) override {
        if (updateTime - data.lastUpdate < AIR_ALERT_UPDATE_PERIOD) return;

        Log::info("Air alert data was updated at: ", String(updateTime));
        data.alertActive = parseRegionAlertNow(httpGet(AIR_ALERT_URL));
        data.lastUpdate = updateTime;
    }
};

#endif
