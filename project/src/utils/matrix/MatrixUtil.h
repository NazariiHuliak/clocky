#ifndef MATRIXUTIL_H
#define MATRIXUTIL_H

#include <Arduino.h>

constexpr uint8_t width = 24;
constexpr uint8_t height = 8;
constexpr uint8_t blockSize = 8;


uint16_t getMatrixAbsolutePosition(uint16_t realIndex);

#endif 