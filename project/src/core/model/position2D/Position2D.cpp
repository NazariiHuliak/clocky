#include "Position2D.h"

Position2D::Position2D(uint16_t x, uint16_t y) {
  this->x = x;
  this->y = y;
}

void Position2D::println() const {
  Serial.println("(" + String(x) + ", " + String(y) + ")");
}

uint16_t Position2D::getX() const {
  return this->x;
}

uint16_t Position2D::getY() const {
  return this->y;
}