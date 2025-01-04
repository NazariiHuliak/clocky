#include <FastLED.h>

// leds set up
// #define MATRIX_WIDTH 24
// #define MAXTRIX_HEIGHT 8

#define DATA_PIN    9
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    192
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          10
#define FRAMES_PER_SECOND  60

class LedMatrix {
  private: 
    uint16_t width;
    uint16_t height;
    uint16_t blockSize;
  public:
    LedMatrix(uint16_t width, uint16_t height, uint16_t blockSize) {
      this->width = width;
      this->height = height;
      this->blockSize = blockSize;
    }

    uint16_t getMatrixIndex(uint16_t realIndex) {
      uint16_t rowNumber = realIndex / width;
      uint16_t rowPosition = realIndex - (rowNumber * width);

      uint16_t blockNumber = rowPosition / blockSize;
      uint16_t blockRowPosition = rowPosition - (blockNumber * blockSize);
      
      return blockNumber * height * blockSize + rowNumber * blockSize + blockRowPosition;
    }
};

LedMatrix ledMatrix(24, 8, 8);

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  delay(3000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

}

void loop() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[ledMatrix.getMatrixIndex(i)] = CHSV(0, 255,255);
    // leds[i] = CHSV(0, 255,255);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
}

class Position2D {
  private:
    uint16_t x;
    uint16_t y;
  public:
    Position2D(uint16_t x, uint16_t y) {
      this->x = x;
      this->y = y;
    }

    void println() {
      Serial.println("(" + String(x) + ", " + String(y) + ")");
    }
};

// class LedMatrix {
//   private: 
//     uint16_t width;
//     uint16_t height;
//     uint16_t blockSize;
//   public:
//     LedMatrix(uint16_t width, uint16_t height, uint16_t blockSize) {
//       this->width = width;
//       this->height = height;
//     }

//     // Position2D getMatrixPosition(Position2D realPosition) {
//     //   return realPosition; 
//     // }

//     uint16_t getMatrixIndex(uint16_t realIndex) {
//       uint16_t rowNumber = realIndex / width;
//       uint16_t rowPosition = realIndex - (rowNumber * width);

//       uint8_t blockNumber = rowPosition / blockSize;
//       uint8_t blockRowPosition = rowPosition - (blockNumber * blockSize);
      
//       return blockNumber * height * blockSize + rowNumber * blockSize + blockRowPosition;
//     }
// };
