#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "../src/utils/log/Log.h"

String httpGet(const String &url) {
    if (WiFi.status() != WL_CONNECTED) {
        Log::error("No internet connection. Cannot perform HTTP GET.");
        return "";
    }

    HTTPClient http;
    String payload = "";

    http.setTimeout(2000);
    http.begin(url);

    int httpCode = http.GET();
    if (httpCode <= 0) {
        Log::error("HTTP GET failed: ", String(httpCode), ". URL: ", url);
    } else if (httpCode >= 200 && httpCode < 300) {
        payload = http.getString();
        Log::info("Successfully fetched. HTTP: ", String(httpCode), ". URL: ", url);
    } else {
        Log::error("Failed to fetch. HTTP: ", String(httpCode), ". Message: " + String(http.getString()), ". URL: " + url);
    }

    http.end();
    return payload;
}

#endif
