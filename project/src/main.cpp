#include <Arduino.h>
#include <Wire.h>
#include <iarduino_RTC.h>
#include <FastLED.h>

#include <../src/model/LedMatrix/LedMatrix.h>
#include <../src/model/watchface/TimeWatchFace.h>

// Leds set up
#define DATA_PIN    9
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         2
#define FRAMES_PER_SECOND  60

iarduino_RTC clock(RTC_DS1302, 2, 4, 3);
LedMatrix ledMatrix;

WatchFace* watchFaces[1] = {
  new TimeWatchFace(leds, clock)
};

void getCompileDateTime(uint16_t& year, uint8_t& month, uint8_t& day, uint8_t& hour, uint8_t& minute, uint8_t& second) {
  const char monthNames[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char monthStr[4] = { __DATE__[0], __DATE__[1], __DATE__[2], '\0' };
  month = (strstr(monthNames, monthStr) - monthNames) / 3 + 1;
  day = atoi(&__DATE__[4]);
  year = atoi(&__DATE__[7]);

  hour = atoi(&__TIME__[0]);
  minute = atoi(&__TIME__[3]);
  second = atoi(&__TIME__[6]);
}

void setup() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
  delay(300);
  clock.begin(&Wire);

  // uint16_t year;
  // uint8_t month, day, hour, minute, second;

  // getCompileDateTime(year, month, day, hour, minute, second);
  // clock.settime(second, minute, hour, day, month, year, 7);
  // Serial.println(hour);
  // Serial.println(minute);
}

void loop() {
  if(millis()%1000==0) {
    ledMatrix.setWatchFace(watchFaces[0], false);
    FastLED.show();
    delay(1);
  }
}