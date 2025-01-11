#include "Position2D.h"

Position2D::Position2D(uint16_t x, uint16_t y) {
  this->x = x;
  this->y = y;
}

void Position2D::println() {
  Serial.println("(" + String(x) + ", " + String(y) + ")");
}

uint16_t Position2D::getX() {
  return this->x;
}

uint16_t Position2D::getY() {
  return this->y;
}