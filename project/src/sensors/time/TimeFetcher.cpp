#include "TimeFetcher.h"

TimeFetcher::TimeFetcher(iarduino_RTC& rtc): clock(rtc) {}

uint8_t* TimeFetcher::getCurrentTime() {
    String timeStr = clock.gettime("H:i");

    uint8_t hours = timeStr.substring(0, 2).toInt();
    uint8_t minutes = timeStr.substring(3, 5).toInt();
    
    if (hours != prevTime[0] || minutes != prevTime[1]) {
        prevTime[0] = hours;
        prevTime[1] = minutes;
    }
    
    return prevTime;
}

uint8_t* TimeFetcher::getCurrentDate() {
    String timeStr = clock.gettime("d-m");

    uint8_t date = timeStr.substring(0, 2).toInt();
    uint8_t month = timeStr.substring(3, 5).toInt();
    
    if (date != prevDate[0] || month != prevDate[1]) {
        prevDate[0] = date;
        prevDate[1] = month;
    }
    
    return prevDate;
}