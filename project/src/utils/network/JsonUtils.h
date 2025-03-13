#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <../src/resources/Config.h>
#include "../src/utils/log/Log.h"


/**
 * Parses the given JSON payload and retrieves the `alertnow` value
 * for the specified region. Returns true if alert is active,
 * false if not, or if an error occurs.
 *
 * @param payload The JSON string to parse.
 * @return        Boolean indicating whether alertnow is true for region specified in the Config.h.
 */
inline bool parseRegionAlertNow(const String &payload) {
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Log::error("parseRegionAlertNow: Deserialization failed: ", String(error.f_str()));
        return false;
    }

    if (!doc["states"].is<JsonObject>()) {
        Log::error("parseRegionAlertNow: No 'states' key in JSON: ");
        return false;
    }

    JsonObject states = doc["states"].as<JsonObject>();
    if (!states[AIR_ALERT_REGION].is<JsonObject>()) {
        Log::error("parseRegionAlertNow: Region not found: ", String(AIR_ALERT_REGION));
        return false;
    }

    JsonObject regionObj = states[AIR_ALERT_REGION].as<JsonObject>();
    if (!regionObj["alertnow"].is<bool>()) {
        Log::error("parseRegionAlertNow: No 'alertnow' key for the region");
        return false;
    }

    bool alertNow = regionObj["alertnow"];
    return alertNow;
}

#endif
