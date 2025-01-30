#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
private:
    struct ButtonState {
        uint8_t pin;
        unsigned long lastClickedTime;
        bool stateChanged;
    };

    ButtonState* buttons;
    uint8_t buttonCount;
    unsigned long checkPeriod;
    unsigned long clickPeriod;
    unsigned long lastTimeChecked;

public:
    ButtonHandler(const uint8_t* buttonPins, uint8_t buttonCount, unsigned long checkPeriod = 100, unsigned long clickPeriod = 500);
    ~ButtonHandler();

    uint8_t processButtons();
};

#endif
