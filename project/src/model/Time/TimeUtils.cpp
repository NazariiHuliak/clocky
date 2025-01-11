#include "TimeUtils.h"

DateTime parseDateTime(const String& timeStr) {
    DateTime t;

    t.day = timeStr.substring(0, 2).toInt();
    t.month = timeStr.substring(3, 5).toInt();
    t.year = timeStr.substring(6, 10).toInt();
    t.hour = timeStr.substring(12, 14).toInt();
    t.minute = timeStr.substring(15, 17).toInt();
    t.second = timeStr.substring(18, 20).toInt();
    t.dayOfWeek = timeStr.substring(22).toInt(); // Assuming D is numeric

    return t;
}

uint8_t* getParsedCurrentTime(DateTime currentTime) {
    uint8_t* timeDigits = new uint8_t[4];

    timeDigits[0] = currentTime.hour / 10;
    timeDigits[1] = currentTime.hour % 10;
    timeDigits[2] = currentTime.minute / 10;
    timeDigits[3] = currentTime.minute % 10;

    return timeDigits;
}

uint8_t* getParsedCurrentDate(DateTime currentTime) {
    uint8_t* timeDigits = new uint8_t[4];

    timeDigits[0] = currentTime.day / 10;
    timeDigits[1] = currentTime.day % 10;
    timeDigits[2] = currentTime.month / 10;
    timeDigits[3] = currentTime.month % 10;

    return timeDigits;
}