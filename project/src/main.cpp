#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RtcDS1302.h>

#include <../src/watchface/implementations/TimeWatchFace.h>
#include <../src/watchface/implementations/TemperatureWatchFace.h>
#include <../src/watchface/implementations/StopwatchWatchFace.h>
#include <../src/hardware/buttons/ButtonHandler.h>
#include <../src/watchface/manager/WatchFaceManager.h>

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

WatchFace* watchFaces[3] = {
  new TimeWatchFace(leds, rtc),
  new TemperatureWatchFace(leds, tempSensor),
  new StopwatchWatchFace(leds)
};
WatchFaceManager watchFaceManager(watchFaces, 3);

void setup() {
  // Leds
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(2);
  FastLED.setDither(0);

  // RTC
  // RtcDateTime compiled = RtcDateTime("Feb 09 2025", "15:22:00");
  rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  rtc.SetDateTime(compiled);

  //Temperature
  tempSensor.begin();

  Serial.begin(115200);
}

void loop() {
  if (watchFaceManager.getIsWatchFaceChangeAllowed()) {
    watchFaceManager.updateAll();

    switch (buttonHandler.processButtons()) {
    case 0:
      watchFaceManager.previousWatchFace();
      Serial.println("1");
      break;
    case 1:
      watchFaceManager.nextMode();
      Serial.println("2");
      break;
    case 2:
      watchFaceManager.nextWatchFace();
      Serial.println("3");
      break;
    default:
      break;
    }
  }

  if (millis() % 10 == 0) {
    watchFaceManager.showWatchFace();
    FastLED.show();
  }
}