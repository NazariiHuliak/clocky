#ifndef POSITION2D_H
#define POSITION2D_H

#include <Arduino.h> // Required for `String` and `Serial`

class Position2D {
  private:
    uint16_t x;
    uint16_t y;

  public:
    Position2D(uint16_t x = 0, uint16_t y = 0);
    void println();
};

#endif 