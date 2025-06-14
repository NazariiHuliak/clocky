#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS A2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, HIGH);

  sensors.begin();
}

void loop(void){ 
  sensors.requestTemperatures(); 
  
  Serial.print("Celsius temperature: ");
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  delay(1000);
}