#ifndef SPECIAL_H
#define SPECIAL_H

#include "Arduino.h"

const byte specialSymbols[4][5] = {
    {
        // .
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01000000
    },

    {
        // !
        0b01000000,
        0b01000000,
        0b01000000,
        0b00000000,
        0b01000000
    },
    {
        // /
        0b00100000,
        0b00100000,
        0b01000000,
        0b10000000,
        0b10000000
    },
    {
        // ?
        0b11000000,
        0b00100000,
        0b01000000,
        0b00000000,
        0b01000000
    }
};

const byte *getSpecialSignPattern(char symbol) {
    if (symbol == '.') return specialSymbols[0];
    if (symbol == '!') return specialSymbols[1];
    if (symbol == '/') return specialSymbols[2];

    return specialSymbols[3];
}

#endif
