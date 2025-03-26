#ifndef AIRALERTCACHE_H
#define AIRALERTCACHE_H

#include <Arduino.h>
#include "../src/resources/Config.h"
#include "../src/utils/log/Log.h"
#include "../src/core/model/network/AirAlert.h"
#include <../src/utils/json/JsonUtils.h>
#include <../src/utils/network/NetworkUtils.h>
#include <../src/core/state/Updatable.h>
#include "../src/core/state/Stateful.h"

class AirAlertCache : public Updatable {
    Stateful<AirAlert> statefulData;

public:
    AirAlertCache() {
    };

    ~AirAlertCache() override = default;

    AirAlert get() {
        return statefulData.data;
    }

    void update(unsigned long updateTime) override {
        if (!canBeUpdateAt(updateTime)) return;

        statefulData.isUpdating = true;
        sendGetRequest(AIR_ALERT_HOST, AIR_ALERT_PORT, AIR_ALERT_ENDPOINT, &statefulData, parseRegionAlert, nullptr);
        Log::info("Air alert: fetch request at: ", String(updateTime));
    }

    bool isUpdating() override {
        return statefulData.isUpdating;
    }

    bool canBeUpdateAt(unsigned long updateTime) override {
        return
                !statefulData.isUpdating &&
                (updateTime - statefulData.data.lastUpdate >= AIR_ALERT_UPDATE_PERIOD || statefulData.data.lastUpdate == 0) &&
                getPermits() > 0;
    }
};

#endif
