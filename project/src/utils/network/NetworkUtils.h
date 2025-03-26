#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "../src/utils/log/Log.h"
#include "../src/core/state/Stateful.h"

size_t getPermits();

bool isWiFiConnected();

void connectToWiFi(const char *ssid, const char *password);

typedef void (*ParseFunction)(String &, void *, void *);
void sendGetRequest(String host, uint16_t port, String endpoint, void *statefulDataStruct,
                    ParseFunction parseFunction, void *parseParams);

#endif
