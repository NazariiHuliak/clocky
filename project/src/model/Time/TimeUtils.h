#ifndef TIME_PARSER_H
#define TIME_PARSER_H

#include <Arduino.h>
#include <iarduino_RTC.h>

class TimeUtils {
  private:
    iarduino_RTC& clock;
    uint8_t prevTime[2];  
    uint8_t prevDate[2];

  public:
    TimeUtils(iarduino_RTC& rtc); 
    uint8_t* getCurrentTime();
    uint8_t* getCurrentDate();
};

#endif