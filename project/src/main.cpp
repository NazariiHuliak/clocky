#include <../src/resources/Config.h>

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RtcDS1302.h>
#include <WiFi.h>

#include <../src/watchface/manager/WatchFaceManager.h>
#include <../src/watchface/feature/Time/TimeWatchFace.h>
#include <../src/watchface/feature/Temperature/TemperatureWatchFace.h>
#include <../src/watchface/feature/Stopwatch/StopwatchWatchFace.h>
#include <../src/watchface/feature/Timer/TimerWatchFace.h>
#include <../src/watchface/feature/AirAlert/AirAlertWatchFace.h>

#include "data/network/NetworkDataManager.h"
#include <data/buttons/ButtonHandler.h>
#include <data/brightness/BrightnessHandler.h>

// Leds
CRGB leds[NUM_LEDS];

// RTC (real time clock)
ThreeWire myWire(DAT, CLK, RST); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

// DS18b20 (temperature)
OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature tempSensor(&oneWire);

// Buttons
const uint8_t buttonPins[] = {BUTTON_1, BUTTON_2, BUTTON_3};
ButtonHandler buttonHandler(buttonPins, NUM_BUTTONS);

BrightnessHandler brightnessHandler(A0);

WatchFace *watchFaces[5] = {
    new TimeWatchFace(leds, rtc),
    new TemperatureWatchFace(leds, tempSensor),
    new StopwatchWatchFace(leds),
    new TimerWatchFace(leds, buttonHandler),
    new AirAlertWatchFace(leds)
};
WatchFaceManager watchFaceManager(watchFaces, 5);

void setup() {
    Serial.begin(115200);

    // Leds
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(2);
    FastLED.setDither(0);

    // RTC
    rtc.Begin();
    // RtcDateTime compiled = RtcDateTime("Mar 15 2025", "11:56:00");
    // // RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // rtc.SetDateTime(compiled);

    // Temperature
    tempSensor.begin();

    // Watchface manager
    watchFaceManager.begin();

    // TODO: Show loading window when connection to the WiFi
    connectToWiFi(SSID_, PASSWORD_);
}

void loop() {
    if (watchFaceManager.getIsWatchFaceChangeAllowed()) {
        switch (buttonHandler.processButtons()) {
            case 1:
                watchFaceManager.previousWatchFace();
                break;
            case 2:
                watchFaceManager.nextMode();
                break;
            case 3:
                watchFaceManager.nextWatchFace();
                break;
            case 5:
                watchFaceManager.resetCurrentWatchFace();
            default:
                break;
        }
    }

    if (millis() % 10 == 0) {
        watchFaceManager.showWatchFace();
        FastLED.show();
    }

    brightnessHandler.update();
    // if (isWiFiConnected()) NetworkDataManager::instance().updateEmergencyData();
    if (isWiFiConnected())NetworkDataManager::instance().updateAll();
}