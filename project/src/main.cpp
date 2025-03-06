#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RtcDS1302.h>

#include <../src/watchface/manager/WatchFaceManager.h>
#include <../src/watchface/feature/Time/TimeWatchFace.h>
#include <../src/watchface/feature/Temperature/TemperatureWatchFace.h>
#include <../src/watchface/feature/Stopwatch/StopwatchWatchFace.h>
#include <../src/watchface/feature/Timer/TimerWatchFace.h>

#include <../src/hardware/buttons/ButtonHandler.h>
#include <hardware/brightness/BrightnessHandler.h>

// Leds
#define DATA_PIN    A5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         2
#define FRAMES_PER_SECOND  60

// RTC (real time clock)
#define RST 21
#define CLK 10
#define DAT 20
ThreeWire myWire(DAT, CLK, RST); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

// DS18b20 (temperature)
#define TEMPERATURE_PIN 6
OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature tempSensor(&oneWire);

// Buttons
#define BUTTON_1 A2
#define BUTTON_2 8
#define BUTTON_3 9

#define NUM_BUTTONS 3

const uint8_t buttonPins[] = { BUTTON_1, BUTTON_2, BUTTON_3 };
ButtonHandler buttonHandler(buttonPins, NUM_BUTTONS);
BrightnessHandler brightnessHandler(A0);

WatchFace* watchFaces[4] = {
  new TimeWatchFace(leds, rtc),
  new TemperatureWatchFace(leds, tempSensor),
  new StopwatchWatchFace(leds),
  new TimerWatchFace(leds, buttonHandler)
};
WatchFaceManager watchFaceManager(watchFaces, 4);


void setup() {
  // Leds
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(2);
  FastLED.setDither(0);

  // RTC
  rtc.Begin();
  // RtcDateTime compiled = RtcDateTime("Feb 10 2025", "18:17:00");
  // RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  // rtc.SetDateTime(compiled);

  //Temperature
  tempSensor.begin();
  Serial.begin(115200);
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

  watchFaceManager.update();
  brightnessHandler.update();
}