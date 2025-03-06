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

#include <../src/resources/Config.h>

// Leds
CRGB leds[NUM_LEDS];

// RTC (real time clock)
ThreeWire myWire(DAT, CLK, RST); // IO, SCLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

// DS18b20 (temperature)
OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature tempSensor(&oneWire);

// Buttons
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