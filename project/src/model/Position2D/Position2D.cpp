#include "Position2D.h"

Position2D::Position2D(uint16_t x, uint16_t y) {
  this->x = x;
  this->y = y;
}

void Position2D::println() {
  Serial.println("(" + String(x) + ", " + String(y) + ")");
}