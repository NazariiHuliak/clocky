#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(const uint8_t* buttonPins, uint8_t buttonCount, unsigned long checkPeriod, unsigned long clickPeriod)
    : buttonCount(buttonCount), checkPeriod(checkPeriod), clickPeriod(clickPeriod), lastTimeChecked(0) {
    buttons = new ButtonState[buttonCount];
    for (uint8_t i = 0; i < buttonCount; i++) {
        pinMode(buttonPins[i], INPUT);
        digitalWrite(buttonPins[i], HIGH);
        buttons[i] = { buttonPins[i], 0, false };
    }
}

ButtonHandler::~ButtonHandler() {
    delete[] buttons;
}

uint8_t ButtonHandler::processButtons() {
    unsigned long currentTime = millis();
    if (currentTime - lastTimeChecked >= checkPeriod) {
        lastTimeChecked = currentTime;

        for (uint8_t i = 0; i < buttonCount; i++) {
            int currentState = digitalRead(buttons[i].pin);

            if (!currentState && !buttons[i].stateChanged) {
                buttons[i].stateChanged = true;

                return i;
            } else if (currentState && buttons[i].stateChanged) {
                if (currentTime - buttons[i].lastClickedTime >= clickPeriod) {
                    buttons[i].lastClickedTime = currentTime;
                    buttons[i].stateChanged = false;
                }
            }
        }
    }

    return -1;
}