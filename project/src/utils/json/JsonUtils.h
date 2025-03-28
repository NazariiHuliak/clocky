#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <../src/resources/Config.h>
#include "../src/utils/log/Log.h"
#include "core/state/Stateful.h"
#include "utils/currency/CurrencyUtils.h"
#include "../src/core/model/network/AirAlert.h"
#include "../src/core/model/network/CurrencyExchange.h"

inline void deleteHeaders(String &payload) {
    int headerEnd = payload.indexOf("\r\n\r\n");
    if (headerEnd != -1) {
        payload = payload.substring(headerEnd + 4);
    }
}

inline void cleanPayload(String &payload) {
    deleteHeaders(payload);
    int start = payload.indexOf("{");
    int end = payload.lastIndexOf("}");

    if (start == -1 || end == -1) {
        Serial.println("Invalid JSON data!");
        return;
    }

    payload = payload.substring(start, end + 1);
    payload.trim();
}

/**
 * Parses the given JSON payload and retrieves the `alertnow` value
 * for the specified region. Returns true if alert is active,
 * false if not, or if an error occurs.
 *
 * @param payload The JSON string to parse.
 * @return        Boolean indicating whether alertnow is true for region specified in the Config.h.
 */
inline void parseRegionAlert(String &payload, void *parseParams, void *statefulObj) {
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
        Log::error("parseRegionAlert: Deserialization failed: ", String(error.f_str()));
        return;
    }

    if (!doc["states"].is<JsonObject>()) {
        Log::error("parseRegionAlert: No 'states' key in JSON: ");
        return;
    }

    JsonObject states = doc["states"].as<JsonObject>();
    if (!states[AIR_ALERT_REGION].is<JsonObject>()) {
        Log::error("parseRegionAlert: Region not found: ", String(AIR_ALERT_REGION));
        return;
    }

    JsonObject regionObj = states[AIR_ALERT_REGION].as<JsonObject>();
    if (!regionObj["alertnow"].is<bool>()) {
        Log::error("parseRegionAlert: No 'alertnow' key for the region");
        return;
    }

    bool alertNow = regionObj["alertnow"];
    unsigned long updateTime = millis();
    statefulAlertData->data.alertActive = alertNow;
    statefulAlertData->data.lastUpdate = updateTime;
    Log::info("Air alert: Status:", alertNow ? "true" : "false", ". Data fetched at: ", String(updateTime));
}

inline std::map<Currency, double> parseCurrencyExchange(String &payload,
                                                        Pair<Currency, std::vector<Pair<Currency, bool>>> *
                                                        parseParams) {
    std::map<Currency, double> exchangeRates;
    cleanPayload(payload);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Log::error("parseCurrencyExchange: Deserialization failed: ", String(error.f_str()));
        return exchangeRates;
    }

    if (!doc["rates"].is<JsonObject>()) {
        Log::error("parseCurrencyExchange: 'rates' not found or invalid in JSON payload.");
        return exchangeRates;
    }

    JsonObject ratesObj = doc["rates"].as<JsonObject>();

    for (const auto &paramPair: parseParams->value) {
        Currency cur = paramPair.key;
        String curStr = CurrencyUtils::toString(cur);

        if (ratesObj[curStr.c_str()].is<double>()) {
            double rate = ratesObj[curStr.c_str()].as<double>();
            exchangeRates[cur] = rate;
        }
    }

    return exchangeRates;
}

inline void parseCurrencyExchange(String &payload, void *parseParams, void *statefulObj) {
    Stateful<CurrencyExchange> *statefulCurrencyExchange = static_cast<Stateful<CurrencyExchange> *>(statefulObj);
    Pair<Currency, std::vector<Pair<Currency, bool> > > *params =
            static_cast<Pair<Currency, std::vector<Pair<Currency, bool> > > *>(parseParams);
    struct Cleanup {
        Stateful<CurrencyExchange> *data;

        ~Cleanup() {
            data->isUpdating = false;
            data->data.lastUpdate = millis();
        }
    } guard{statefulCurrencyExchange};

    std::map<Currency, double> exchangeRates = parseCurrencyExchange(payload, params);

    Currency baseCurrency = params->key;
    uint8_t currentRateIndex = statefulCurrencyExchange->data.numCurrencies;

    for (const auto &targetCurrency: params->value) {
        auto it = exchangeRates.find(targetCurrency.key);
        if (it != exchangeRates.end()) {
            double exchangeRate = targetCurrency.value ? 1.0 / it->second : it->second;
            statefulCurrencyExchange->data.exchangeRate[currentRateIndex] =
                    ExchangeRate{targetCurrency.key, baseCurrency, exchangeRate};
            currentRateIndex++;
        } else {
            Log::error("Currency not found in exchangeRates map: ", CurrencyUtils::toString(targetCurrency.key));
        }
    }

    unsigned long updateTime = millis();
    statefulCurrencyExchange->data.numCurrencies = currentRateIndex;
    statefulCurrencyExchange->data.lastUpdate = updateTime;
    Log::info("Currency exchange: request for ", CurrencyUtils::toString(baseCurrency),
              ". Data fetched at: ", String(updateTime));

    for (uint8_t i = 0; i < statefulCurrencyExchange->data.numCurrencies; i++) {
        const ExchangeRate &rate = statefulCurrencyExchange->data.exchangeRate[i];
        Serial.printf("From: %s, To: %s, Rate: %.4f\n", CurrencyUtils::toString(rate.fromCurrency),
                      CurrencyUtils::toString(rate.toCurrency), rate.rate);
    }
}

inline void parseServerData(const String &body, Pair<String, String> &wifiDetails, std::vector<Pair<String, String>> &currencyPairs) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        Log::error("parseServerData: Deserialization failed: ", String(error.f_str()));
        return;
    }

    wifiDetails.key = doc["ssid"].as<String>();
    wifiDetails.value = doc["password"].as<String>();
    Serial.print("Received SSID: ");
    Serial.println(wifiDetails.key);
    Serial.print("Received Password: ");
    Serial.println(wifiDetails.value);

    // Parse currency pairs

    if (!doc["currencyPairs"].is<JsonArray>()) {
        Log::error("parseServerData: 'currencyPairs' not found or invalid in JSON payload.");
        return;
    }

    JsonArray currencyPairArray = doc["currencyPairs"].as<JsonArray>();
    currencyPairs.clear();

    for (JsonVariant pair : currencyPairArray) {
        String from = pair["from"].as<String>();
        String to = pair["to"].as<String>();
        if (from.length() != 3 || to.length() != 3) {
            Log::error("parseServerData: 'from' or 'to' are invalid length.");
            continue;
        }
        currencyPairs.push_back(Pair<String, String>(from, to));
        Serial.print(from + " → " + to);
        Serial.println();
    }
}

#endif
