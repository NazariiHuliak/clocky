#include "MatrixUtil.h"

uint16_t getMatrixAbsolutePosition(uint16_t realIndex) {
    uint16_t reversedRealIndex = width * height - realIndex - 1;

    uint16_t rowNumber = reversedRealIndex / width;
    uint16_t rowPosition = reversedRealIndex - (rowNumber * width);

    uint16_t blockNumber = rowPosition / blockSize;
    uint16_t blockRowPosition = rowPosition - (blockNumber * blockSize);
    
    return blockNumber * height * blockSize + rowNumber * blockSize + blockRowPosition;
}