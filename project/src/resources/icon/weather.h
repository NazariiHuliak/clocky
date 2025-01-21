#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

const uint8_t iconHeight = 8;
const uint8_t iconWidth = 7;

const uint8_t sun[8][7] = 
        {{0, 0, 0, 0, 0, 0, 0}, 
        {0, 1, 1, 1, 0, 0, 0}, 
        {1, 1, 2, 1, 1, 0, 0}, 
        {1, 2, 1, 1, 1, 1, 0}, 
        {1, 2, 1, 1, 1, 1, 0}, 
        {1, 2, 1, 1, 1, 1, 0}, 
        {1, 1, 2, 1, 1, 0, 0}, 
        {0, 1, 1, 1, 0, 0, 0}};

// uint8_t cloud[iconHeight][iconWidth] = 
//         {{1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1},
//          {1, 1, 1, 1, 1, 1}};

#endif 
