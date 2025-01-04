#include <Arduino.h>
#include <FastLED.h>
#include <../src/model/BlockMatrix.h>

// Leds set up
#define DATA_PIN    9
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         10
#define FRAMES_PER_SECOND  60

BlockMatrix ledMatrix(24, 8, 8);
void setup() {
  Serial.begin(9600);
  delay(3000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[ledMatrix.getMatrixIndex(i)] = CHSV(0, 0,255);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
}