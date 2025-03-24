#include "NetworkUtils.h"

size_t permits = 1;

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void connectToWiFi(const char *ssid, const char *password) {
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

void sendGetRequest(const char *host, uint16_t port, const char *endpoint, void *statefulDataStruct,
                    ParseFunction parseFunction) {
    // TODO: Check whether dataStruct is inherited from Stateful

    if (!permits) return;
    if (!isWiFiConnected()) {
        Log::error("No internet connection. Cannot perform HTTP GET.");
        return;
    }

    Log::info("sendGetRequest. Permits: ", permits);
    AsyncClient *client = new AsyncClient;
    String *payloadBuffer = new String();
    bool headersEnded = false;

    client->onError([client, payloadBuffer](void *arg, AsyncClient *client, int8_t error) {
        Log::error("sendGetRequest. Error: ", client->errorToString(error));
        client->close(true);
        delete client;
        delete payloadBuffer;
    });

    client->onConnect(
        [host, endpoint, statefulDataStruct, parseFunction, payloadBuffer, &headersEnded](void *arg, AsyncClient *client) {
            permits--;
            Log::info("sendGetRequest. Connected to ", host, ".");

            client->onDisconnect([payloadBuffer, statefulDataStruct, parseFunction](void *arg, AsyncClient *client) {
                Log::info("sendGetRequest. Disconnected.");

                // Log::info("sendGetRequest. Payload: \n", *payloadBuffer);
                if (parseFunction && !payloadBuffer->isEmpty()) {
                    parseFunction(*payloadBuffer, statefulDataStruct);
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

            const String request = "GET " + String(endpoint) + " HTTP/1.1\r\n"
                                   "Host: " + String(host) + "\r\n"
                                   "User-Agent: ESP32\r\n"
                                   "Connection: close\r\n\r\n";

            client->write(request.c_str());
        });

    if (!client->connect(host, port)) {
        Log::error("sendGetRequest. Connection failed. Host: ", host, ")");
        delete client;
        delete payloadBuffer;
    }
}