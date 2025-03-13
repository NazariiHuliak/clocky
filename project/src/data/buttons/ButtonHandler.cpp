#include "ButtonHandler.h"


ButtonHandler::ButtonHandler(
    const uint8_t *buttonPins,
    const uint8_t buttonCount,
    const unsigned long checkPeriod,
    const unsigned long clickPeriod)
    : buttonCount(buttonCount), checkPeriod(checkPeriod), clickPeriod(clickPeriod), lastTimeChecked(0) {
    buttons = new ButtonState[buttonCount];
    for (uint8_t i = 0; i < buttonCount; i++) {
        pinMode(buttonPins[i], INPUT);
        digitalWrite(buttonPins[i], HIGH);
        buttons[i] = {buttonPins[i], 0, false, false};
    }
}

ButtonHandler::~ButtonHandler() {
    delete[] buttons;
}

bool ButtonHandler::shouldUpdate(unsigned long currentTime) {
    if (currentTime - lastTimeChecked >= checkPeriod) {
        lastTimeChecked = currentTime;
        return true;
    }

    return false;
}

/**
    @return 0: nothing clicked <br>
    i + 1: button_i was clicked <br>
    buttonCount + i + 1: button_i was pressed for 1 second <br>
*/
uint8_t ButtonHandler::processButtons() {
    unsigned long currentTime = millis();
    if (!shouldUpdate(currentTime)) return 0;

    for (uint8_t i = 0; i < buttonCount; i++) {
        bool pressed = !static_cast<bool>(digitalRead(buttons[i].pin));
        unsigned long timePeriod = currentTime - buttons[i].pressStartTime;

        if (pressed) {
            if (!buttons[i].wasClicked && timePeriod >= clickPeriod) {
                buttons[i].wasClicked = true;
                buttons[i].pressStartTime = currentTime;
            } else {
                if (!buttons[i].wasHolded && timePeriod >= HOLD_TIME_THRESHOLD) {
                    buttons[i].wasHolded = true;

                    return buttonCount + i + 1;
                }
            }
        } else {
            if (buttons[i].wasClicked) {
                buttons[i].wasClicked = false;
                if (!buttons[i].wasHolded) {
                    buttons[i].pressStartTime = currentTime;

                    return i + 1;
                } else {
                    buttons[i].wasHolded = false;
                }
            }
        }
    }

    return 0;
}