#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "../src/utils/log/Log.h"
#include "../src/core/state/Stateful.h"

bool isWiFiConnected();

void connectToWiFi(const char *ssid, const char *password);

typedef void (*ParseFunction)(String &, void *);
void sendGetRequest(const char *host, uint16_t port, const char *endpoint, void *statefulDataStruct,
                    ParseFunction parseFunction);

#endif
