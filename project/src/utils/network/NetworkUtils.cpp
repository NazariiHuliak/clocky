#include "NetworkUtils.h"

size_t permits = 1;

size_t getPermits() {
    return permits;
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

inline void connectToWiFi(const char *ssid, const char *password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Log::info("Connecting to WiFi...");

    unsigned long startAttemptTime = millis();
    while (!isWiFiConnected() && (millis() - startAttemptTime < CONNECTING_TIMEOUT)) {
        delay(500);
        Log::info("Connecting to WiFi...");
    }

    if (isWiFiConnected()) {
        Log::info("Connected to WiFi!");
    } else {
        Log::warn("Failed to connect to WiFi within timeout.");
    }
}

void connectToWifi(const Pair<String, String> &wifiData) {
    connectToWiFi(wifiData.key.c_str(), wifiData.value.c_str());
}

void setIsUpdatingFalse(void* statefulDataStruct) {
    if (statefulDataStruct) {
        IStateful *stateful = static_cast<IStateful*>(statefulDataStruct);
        stateful->isUpdating = false;
    }
}

void sendGetRequest(String host, uint16_t port, String endpoint, void *statefulDataStruct,
                    ParseFunction parseFunction, void *parseParams) {
    // TODO: Check whether dataStruct is inherited from Stateful

    if (!permits) return;
    if (!isWiFiConnected()) {
        Log::error("No internet connection. Cannot perform HTTP GET.");
        setIsUpdatingFalse(statefulDataStruct);
        return;
    }

    Log::info("sendGetRequest. Permits: ", permits);
    AsyncClient *client = new AsyncClient;
    String *payloadBuffer = new String();
    bool headersEnded = false;

    client->onError([client, payloadBuffer, statefulDataStruct](void *arg, AsyncClient *client, int8_t error) {
        Log::error("sendGetRequest. Error: ", client->errorToString(error));
        client->close(true);

        setIsUpdatingFalse(statefulDataStruct);
        delete client;
        delete payloadBuffer;
    });

    client->onConnect(
        [host, endpoint, statefulDataStruct, parseFunction, parseParams, payloadBuffer, &headersEnded](void *arg, AsyncClient *client) {
            permits--;
            Log::info("sendGetRequest. Connected to ", host, ".");

            client->onDisconnect([payloadBuffer, statefulDataStruct, parseFunction, parseParams](void *arg, AsyncClient *client) {
                Log::info("sendGetRequest. Disconnected.");

                // Log::info("sendGetRequest. Payload: \n", *payloadBuffer);
                if (parseFunction && !payloadBuffer->isEmpty()) {
                    parseFunction(*payloadBuffer, parseParams, statefulDataStruct);
                }

                client->close(true);
                delete client;
                delete payloadBuffer;

                permits++;
            });

            client->onData([payloadBuffer, &headersEnded](void *arg, AsyncClient *client, void *data, size_t len) {
                Log::info("sendGetRequest. Data received: (len=", len, ")");
                payloadBuffer->concat(String(static_cast<char *>(data), len));
            });

            const String request = "GET " + endpoint + " HTTP/1.1\r\n"
                                   "Host: " + host + "\r\n"
                                   "User-Agent: ESP32\r\n"
                                   "Connection: close\r\n\r\n";
            client->write(request.c_str());
        });

    if (!client->connect(host.c_str(), port)) {
        Log::error("sendGetRequest. Connection failed. Host: ", host, ")");

        setIsUpdatingFalse(statefulDataStruct);
        delete client;
        delete payloadBuffer;
    }
}