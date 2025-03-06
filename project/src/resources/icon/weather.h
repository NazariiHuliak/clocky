#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <../src/resources/Config.h>


const uint8_t sun[ICON_HEIGHT][ICON_WIDTH] =
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
