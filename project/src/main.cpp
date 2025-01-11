#include <Arduino.h>
#include <Wire.h>
#include <iarduino_RTC.h>
#include <FastLED.h>
#include <../src/model/BlockMatrix/BlockMatrix.h>
#include "../src/resources/font/digits.h"
#include "../src/model/LedMatrix/LedMatrix.h"
#include "../src/model/Time/TimeUtils.h"

// Leds set up
#define DATA_PIN    9
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         2
#define FRAMES_PER_SECOND  60

LedMatrix ledMatrix(FastLED, leds, 24, 8, 8);
iarduino_RTC clock(RTC_DS1302, 2, 4, 3);

void setup() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
  delay(300);
  clock.begin(&Wire);
}

void loop() {
  if(millis()%1000==0){
    Serial.println(clock.gettime("d-m-Y, H:i:s, D"));
    DateTime dateTime = parseDateTime(clock.gettime("d-m-Y, H:i:s, D"));
    ledMatrix.setDateTimeWatchFace(dateTime, true);
    FastLED.show();
    delay(1);
  }
}