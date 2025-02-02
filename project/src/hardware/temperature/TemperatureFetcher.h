#ifndef TEMPERATUREFETCHER_H
#define TEMPERATUREFETCHER_H

#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureFetcher {
private:
    DallasTemperature& sensor;
    double prevTemp = 0.0;
public:
    TemperatureFetcher(DallasTemperature& sensorObj);
    double getCurrentTemp();
};

#endif
