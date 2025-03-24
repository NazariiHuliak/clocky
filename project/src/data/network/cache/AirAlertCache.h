#ifndef AIRALERTCACHE_H
#define AIRALERTCACHE_H

#include <Arduino.h>
#include "../src/resources/Config.h"
#include "../src/utils/log/Log.h"
#include "../src/core/model/network/AirAlert.h"
#include <../src/utils/network/JsonUtils.h>
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
        if (statefulData.isUpdating || updateTime - statefulData.lastUpdate < AIR_ALERT_UPDATE_PERIOD) return;
        statefulData.isUpdating = true;

        sendGetRequest(AIR_ALERT_HOST, AIR_ALERT_PORT, AIR_ALERT_ENDPOINT, &statefulData, parseRegionAlertNow);
        Log::info("Air alert data was updated at: ", String(updateTime));
    }
};

#endif
