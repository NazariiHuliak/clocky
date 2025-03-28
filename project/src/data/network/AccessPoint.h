#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include "../src/resources/html/SettingsPage.h"
#include "../src/utils/json/JsonUtils.h"
#include "../src/utils/network/NetworkUtils.h"
#include "../src/utils/EPPROM/EEPROMUtils.h"


class AccessPoint {
private:
    AsyncWebServer server;
    bool shouldBeShutDown = false;

public:
    AccessPoint(): server(80) {
    };

    void setShouldBeShutDown(bool state) {
        shouldBeShutDown = state;
    }

    bool getShouldBeShutDown() const {
        return shouldBeShutDown;
    }

    void create(bool initialization) {
        shouldBeShutDown = false;
        WiFi.softAP(LOCAL_SSID_, LOCAL_PASSWORD_);
        IPAddress IP = WiFi.softAPIP();
        Log::info("AP: IP address: ", String(IP));
        Log::info("AP: local ip: ", String(WiFi.localIP()));

        server.on("/", HTTP_GET, [initialization](AsyncWebServerRequest *request) {
            request->send(200, "text/html", getHTMLPage(!initialization));
        });

        server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
                  }, NULL,
                  [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
                      String body(reinterpret_cast<char *>(data), len);

                      Pair<String, String> wifiDetails("", "");
                      std::vector<Pair<String, String> > currencyPairs;
                      parseServerData(body, wifiDetails, currencyPairs);

                      // Serial.println("body: " + String(body));

                      // writeWifiDataInEEPROM(wifiDetails.key, wifiDetails.value);
                      // writeCurrencyPairs(currencyPairs);

                      request->send(200, "text/plain", "SUCCESS");
                      shouldBeShutDown = true;

                      Log::info("AP: Successfully received data");
                  });
        server.begin();
    }

    void shutdown() {
        delay(100);

        server.end();
        WiFi.softAPdisconnect(true);
        Log::info("Access Point shutdown.");

        shouldBeShutDown = false;

        if (isWifiSettingsExistsInEEPROM() && !isWiFiConnected()) {
            Pair<String, String> wifiData = readWifiDataFromEEPROM();
            connectToWiFi(wifiData.key.c_str(), wifiData.value.c_str());
        }
    }

    void end() {
        server.end();
    }
};

#endif
