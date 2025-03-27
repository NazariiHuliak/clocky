#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <vector>
#include "../core/model/Currency.h"
#include <../src/core/model/Pair.h>

// matrix
#define MATRIX_HEIGHT  8
#define MATRIX_WIDTH 24
#define BLOCK_SIZE 8

// icon
#define ICON_HEIGHT 8
#define ICON_WIDTH 7

// Logs
#define LOG_ENABLED true

// Leds
#define DATA_PIN    A5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    192
#define BRIGHTNESS  2

// RTC (real time clock)
#define RST 21
#define CLK 10
#define DAT 20

// DS18b20 (temperature)
#define TEMPERATURE_PIN 6

// Buttons
#define NUM_BUTTONS 3
#define BUTTON_1 A2
#define BUTTON_2 8
#define BUTTON_3 9

// Wifi setting (should be moved to SRAM memory)
#define SSID_ "HOME_2"
#define PASSWORD_ "24092004"
#define CONNECTING_TIMEOUT 5000

// Air alert settings
#define AIR_ALERT_HOST "ubilling.net.ua"
#define AIR_ALERT_PORT 80
#define AIR_ALERT_ENDPOINT "/aerialalerts/"
#define AIR_ALERT_REGION "Львівська область"
#define AIR_ALERT_UPDATE_PERIOD 180000 // 3 min

// Currencies exchange
#define CURRENCY_EXCHANGE_HOST "open.er-api.com"
#define CURRENCY_EXCHANGE_PORT 80
#define CURRENCY_EXCHANGE_ENDPOINT "/v6/latest"
const std::vector<Pair<Currency, Currency> > CURRENCY_EXCHANGE_PAIRS = {
    {USD, UAH},
    {PLN, UAH},
    {PLN, USD}
    /*,
    {PLN, USD},
    {UAH, EUR},
    {USD, EUR}*/
};
#define CURRENCY_EXCHANGE_UPDATE_PERIOD 50000 // should be set to 24 hours
#define CURRENCY_EXCHANGE_FREQUENCY_REQUEST_TIME 2000 // should be set to 1 sec

#endif
