#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <Arduino.h> 

struct DateTime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dayOfWeek;
};

void beginClock();
DateTime parseDateTime(const String& timeStr);
uint8_t* getParsedCurrentTime(DateTime currentTime);
uint8_t* getParsedCurrentDate(DateTime currentTime);

#endif 