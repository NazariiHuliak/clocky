#ifndef TIME_PARSER_H
#define TIME_PARSER_H

#include <Arduino.h>
#include <iarduino_RTC.h>

class TimeFetcher {
  private:
    iarduino_RTC& clock;
    uint8_t prevTime[2];  
    uint8_t prevDate[2];

  public:
    TimeFetcher(iarduino_RTC& rtc); 
    uint8_t* getCurrentTime();
    uint8_t* getCurrentDate();
};

#endif