#ifndef SYMBOLTYPE_H
#define SYMBOLTYPE_H

#include "Arduino.h"

enum SymbolType {
    LETTER,
    DIGIT,
    SPECIAL,
    UNKNOWN
};

SymbolType getSymbolType(char c) {
    if (std::isalpha(c)) {
        return LETTER;
    } else if (std::isdigit(c)) {
        return DIGIT;
    } else {
        return SPECIAL;
    }
}

#endif
