#include "BlockMatrix.h"

BlockMatrix::BlockMatrix(uint16_t width, uint16_t height, uint16_t blockSize) {
  this->width = width;
  this->height = height;
  this->blockSize = blockSize;
}

Position2D BlockMatrix::getPosition(Position2D realPosition) {
  return realPosition;
}

uint16_t BlockMatrix::getMatrixIndex(uint16_t realIndex) {
    uint16_t rowNumber = realIndex / width;
    uint16_t rowPosition = realIndex - (rowNumber * width);

    uint16_t blockNumber = rowPosition / blockSize;
    uint16_t blockRowPosition = rowPosition - (blockNumber * blockSize);
    
    return blockNumber * height * blockSize + rowNumber * blockSize + blockRowPosition;
}