#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>


class ButtonHandler {
    struct ButtonState {
        uint8_t pin;
        unsigned long pressStartTime;
        bool wasClicked;
        bool wasHolded;
    };

    ButtonState* buttons;
    uint8_t buttonCount;
    unsigned long checkPeriod;
    unsigned long clickPeriod;
    unsigned long lastTimeChecked;

    unsigned long HOLD_TIME_THRESHOLD = 1000;

    bool shouldUpdate(unsigned long currentTime);

public:
    ButtonHandler(const uint8_t* buttonPins, uint8_t buttonCount, unsigned long checkPeriod = 100, unsigned long clickPeriod = 100);
    ~ButtonHandler();

    uint8_t processButtons();
};

#endif