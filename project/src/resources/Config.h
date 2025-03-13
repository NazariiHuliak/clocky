#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

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
#define SSID "HOME_2"
#define PASSWORD "24092004"
#define CONNECTING_TIMEOUT 5000

// Air alert settings
#define AIR_ALERT_URL "https://ubilling.net.ua/aerialalerts/"
#define AIR_ALERT_REGION "Львівська область"
#define AIR_ALERT_UPDATE_PERIOD 10000

#endif
