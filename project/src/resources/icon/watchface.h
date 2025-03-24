#ifndef WATCHFACEICON_H
#define WATCHFACEICON_H

#include <Arduino.h>
#include <../src/resources/Config.h>

const uint8_t stopwatch[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 3, 3, 3, 0, 0}, 
    {0, 3, 0, 4, 0, 3, 0}, 
    {3, 0, 0, 4, 0, 0, 3}, 
    {3, 0, 4, 4, 0, 0, 3}, 
    {3, 0, 0, 0, 0, 0, 3}, 
    {0, 3, 0, 0, 0, 3, 0}, 
    {0, 0, 3, 3, 3, 0, 0}};

const uint8_t timer[ICON_HEIGHT][ICON_WIDTH] =
    {{5, 5, 5, 5, 5, 5, 5}, 
    {0, 5, 0, 0, 0, 5, 0}, 
    {0, 5, 1, 1, 1, 5, 0}, 
    {0, 0, 5, 1, 5, 0, 0}, 
    {0, 0, 5, 0, 5, 0, 0}, 
    {0, 5, 0, 1, 0, 5, 0}, 
    {0, 5, 1, 1, 1, 5, 0}, 
    {5, 5, 5, 5, 5, 5, 5}};

const uint8_t thermometer[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 4, 4, 4, 0, 0},
    {0, 0, 4, 0, 4, 0, 0},
    {0, 0, 4, 0, 4, 0, 0},
    {0, 0, 4, 6, 4, 0, 0},
    {0, 0, 4, 6, 4, 0, 0},
    {0, 4, 6, 6, 6, 4, 0},
    {0, 4, 6, 6, 6, 4, 0},
    {0, 0, 4, 4, 4, 0, 0}};

#endif 