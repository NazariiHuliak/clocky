#include <Arduino.h>
#include <Wire.h>
#include <iarduino_RTC.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <../src/watchface/managers/WatchFaceManager.h>
#include <../src/watchface/implementations/TimeWatchFace.h>
#include <../src/watchface/implementations/TemperatureWatchFace.h>
#include <../src/hardware/buttons/ButtonHandler.h>

// Leds
#define DATA_PIN    9
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         2
#define FRAMES_PER_SECOND  60

// RTC (real time clock)
#define RST 2
#define CLK 4
#define DAT 3
iarduino_RTC clock(RTC_DS1302, RST, CLK, DAT);

// DS18b20 (temperature)
#define TEMPERATURE_PIN A2
OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature tempSensor(&oneWire);

// Buttons
#define BUTTON_1 5
#define BUTTON_2 6
#define BUTTON_3 7

#define NUM_BUTTONS 3

const uint8_t buttonPins[] = { BUTTON_1, BUTTON_2, BUTTON_3 };
ButtonHandler buttonHandler(buttonPins, NUM_BUTTONS);


WatchFace* watchFaces[2] = {
  new TimeWatchFace(leds, clock),
  new TemperatureWatchFace(leds, tempSensor)
};
WatchFaceManager watchFaceManager(watchFaces, 2);

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  clock.begin(&Wire);
  tempSensor.begin();
  Serial.begin(9600);
}

void loop() {
  if (watchFaceManager.getIsWatchFaceChangeAllowed()) {
    watchFaceManager.updateWatchFaceData();

    // tempSensor.requestTemperatures();
    // Serial.println(tempSensor.getTempCByIndex(0));

    switch (buttonHandler.processButtons()) {
    case 0:
      watchFaceManager.previousWatchFace();
      break;
    case 1:
      watchFaceManager.nextMode();
      break;
    case 2:
      watchFaceManager.nextWatchFace();
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