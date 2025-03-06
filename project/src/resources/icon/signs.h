#ifndef SIGNS_H
#define SIGNS_H

#include <Arduino.h>
#include <../src/resources/Config.h>

const uint8_t celsiusSign[ICON_HEIGHT][ICON_WIDTH] =
    {{0, 0, 0, 0, 0, 0, 0}, 
    {1, 1, 0, 1, 1, 1, 0}, 
    {1, 1, 0, 1, 0, 0, 0}, 
    {0, 0, 0, 1, 0, 0, 0}, 
    {0, 0, 0, 1, 0, 0, 0}, 
    {0, 0, 0, 1, 1, 1, 0}, 
    {0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0}};

const uint8_t celsiusSignSmall[ICON_HEIGHT][ICON_WIDTH] =
    {{1, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 1, 0, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 0, 0}, 
    {0, 1, 1, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0}};


#endif 