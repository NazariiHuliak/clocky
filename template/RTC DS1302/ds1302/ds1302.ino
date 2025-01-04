#include <Wire.h>
#include <iarduino_RTC.h>
// RST CLK DAT 
iarduino_RTC clock(RTC_DS1302, 2, 4, 3);

void setup(){                                              

  Serial.begin(9600);
  delay(300);
  clock.begin(&Wire);
  // clock.settime(15, 4, 6, 23, 1, 22, 6);
}    

void loop(){

  if(millis()%1000==0){
    Serial.println(clock.gettime("d-m-Y, H:i:s, D"));
    delay(1);
  }

}