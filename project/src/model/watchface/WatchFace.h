#ifndef WATCHFACE_H
#define WATCHFACE_H

#include <FastLED.h>

#include <../src/model/Position2D/Position2D.h>
#include <../src/model/BlockMatrix/MatrixUtil.h>

#include <../src/resources/font/digits.h>
#include <../src/resources/icon/weather.h>
#include <../src/resources/icon/colors.h>


class WatchFace {
    CHSV digitColor{0, 0, 255};
    CRGB* leds;
    unsigned long modeChangeThreshold = 500;
public:
    WatchFace(CRGB* leds) : leds(leds) {}

    virtual void setFrame(bool modeWasChanged) = 0;
    virtual void nextMode() = 0;
    virtual int getNumModes() const = 0;
    virtual unsigned long getLastTimeModeChange() const = 0;
    virtual void setLastTimeModeChange(unsigned long lastTimeModeChange) = 0;

    void processModeChange(bool modeWasChanged) {
        if (getNumModes() == 1) return;

        unsigned long currentTime = millis();
        if (currentTime - getLastTimeModeChange() > modeChangeThreshold && modeWasChanged) {
            setLastTimeModeChange(currentTime);
            nextMode();
        }
    }

    void setDigit(Position2D position2D, uint8_t digit) {
        for (int row = 0; row < 5; row++) {
            byte rowData = numbers[digit][row];

            int pixelRow = (position2D.getY() + row) * 24;
            for (int bitPos = 6; bitPos >= 4; bitPos--) {
                int pixelCol = position2D.getX() + (6 - bitPos);
                int pixelAbsolutPosition = pixelRow + pixelCol; 
            
                if (bitRead(rowData, bitPos)) {
                    leds[getMatrixIndex(pixelAbsolutPosition)] = digitColor;
                } else {
                    leds[getMatrixIndex(pixelAbsolutPosition)] = CHSV(0, 0, 0);
                }
            }
        }
    }

    void setColon(bool turnOn) {
        if (turnOn) {
            setPixel(Position2D(15, 2));
            setPixel(Position2D(15, 4));
        } else {
            setPixel(Position2D(15, 2), CHSV(0, 0, 0));
            setPixel(Position2D(15, 4), CHSV(0, 0, 0));
        }
    }

    void setPixel(Position2D position2D) {
        setPixel(position2D, digitColor);
    }

    void setPixel(Position2D position2D, CHSV color) {
        uint16_t pixelAbsolutPosition = position2D.getY() * width + position2D.getX();
        leds[getMatrixIndex(pixelAbsolutPosition)] = color;
    }

    void setIcon() {
        for (uint8_t row = 0; row < iconHeight; row++) {
            const uint8_t* rowData = sun[row];
            
            int pixelRow = row * width;
            for (int col = 0; col < iconWidth; col++) {  
                int pixelAbsolutPosition = pixelRow + col; 
                int colorIndex = rowData[col];
                
                const uint8_t* color = colors[colorIndex]; 
                leds[getMatrixIndex(pixelAbsolutPosition)] = CRGB(color[0], color[1], color[2]);
            }
        }
    }

    virtual ~WatchFace() {}
};

#endif 