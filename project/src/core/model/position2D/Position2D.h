#ifndef POSITION2D_H
#define POSITION2D_H

#include <Arduino.h>


class Position2D {
  uint16_t x;
  uint16_t y;

public:
  explicit Position2D(uint16_t x = 0, uint16_t y = 0);
  uint16_t getX() const;
  uint16_t getY() const;
  void println() const;
};

#endif 