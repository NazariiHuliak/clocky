#ifndef BLOCKMATRIX_H
#define BLOCKMATRIX_H

#include <Arduino.h>
#include "Position2D.h"

class BlockMatrix {
  private:
    uint16_t width;
    uint16_t height;
    uint16_t blockSize;

  public:
    BlockMatrix(uint16_t width, uint16_t height, uint16_t blockSize);
    Position2D getPosition(Position2D realPosition);
    uint16_t getMatrixIndex(uint16_t realIndex);
};

#endif 