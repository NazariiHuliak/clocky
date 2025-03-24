#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <../src/resources/Config.h>
#include "../src/utils/log/Log.h"
#include "core/state/Stateful.h"

inline void deleteHeaders(String &payload) {
    int headerEnd = payload.indexOf("\r\n\r\n");
    if (headerEnd != -1) {
        payload = payload.substring(headerEnd + 4);
    }
}

/**
 * Parses the given JSON payload and retrieves the `alertnow` value
 * for the specified region. Returns true if alert is active,
 * false if not, or if an error occurs.
 *
 * @param payload The JSON string to parse.
 * @return        Boolean indicating whether alertnow is true for region specified in the Config.h.
 */
inline void parseRegionAlertNow(String &payload, void *statefulObj) {
    Stateful<AirAlert> *statefulAlertData = static_cast<Stateful<AirAlert> *>(statefulObj);
    JsonDocument doc;

    struct Cleanup {
        Stateful<AirAlert> *data;
        ~Cleanup() {
            data->isUpdating = false;
        }
    } guard{statefulAlertData};

    deleteHeaders(payload);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Log::error("parseRegionAlertNow: Deserialization failed: ", String(error.f_str()));
        return;
    }

    if (!doc["states"].is<JsonObject>()) {
        Log::error("parseRegionAlertNow: No 'states' key in JSON: ");
        return;
    }

    JsonObject states = doc["states"].as<JsonObject>();
    if (!states[AIR_ALERT_REGION].is<JsonObject>()) {
        Log::error("parseRegionAlertNow: Region not found: ", String(AIR_ALERT_REGION));
        return;
    }

    JsonObject regionObj = states[AIR_ALERT_REGION].as<JsonObject>();
    if (!regionObj["alertnow"].is<bool>()) {
        Log::error("parseRegionAlertNow: No 'alertnow' key for the region");
        return;
    }

    bool alertNow = regionObj["alertnow"];
    statefulAlertData->alertActive = alertNow;
    statefulAlertData->lastUpdate = millis();
    statefulAlertData->print();
    Log::info("Data fetched");
}

#endif
