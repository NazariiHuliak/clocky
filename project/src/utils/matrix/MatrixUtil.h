#ifndef MATRIXUTIL_H
#define MATRIXUTIL_H

#include <Arduino.h>
#include <../src/resources/Config.h>

inline uint16_t getMatrixAbsolutePosition(uint16_t realIndex) {
    uint16_t reversedRealIndex = MATRIX_WIDTH * MATRIX_HEIGHT - realIndex - 1;

    uint16_t rowNumber = reversedRealIndex / MATRIX_WIDTH;
    uint16_t rowPosition = reversedRealIndex - (rowNumber * MATRIX_WIDTH);

    uint16_t blockNumber = rowPosition / BLOCK_SIZE;
    uint16_t blockRowPosition = rowPosition - (blockNumber * BLOCK_SIZE);

    return blockNumber * MATRIX_HEIGHT * BLOCK_SIZE + rowNumber * BLOCK_SIZE + blockRowPosition;
}

#endif 