#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 2; i < 14; i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i, HIGH);
  }
  Wire.begin();
  RTC.begin();
  // If we remove the comment from the following line, we will set up the module time and date with the computer one
  RTC.adjust(DateTime(__DATE__, __TIME__));
}

int hr = 0;

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = RTC.now();
  int hour = now.hour();
  if(now.hour()>12){
    hour = now.hour() - 12;
  }
  int min = now.minute()/5;
  int exMin = now.minute()%5;
  int sec = now.second()/5;
  int exSec = now.second()%5;
  int del = 0;

  bool hrchange = hr != hour;
  // === Start a hourly animation if there was a change in hours.
  if(hrchange){
    // 5000 ms animation
    // blink twice
    for(int i = 0; i<2; i++){
      for(int j = 2; j<14; j++){
        digitalWrite(j, LOW);
      }
      delay(50);
      for(int j = 2; j<14; j++){
        digitalWrite(j, HIGH);
      }
      delay(50);
    }
    // 200 ms
    // Circle fill clockwise.
    for(int i = 2; i<14; i++){
      digitalWrite(i, LOW);
      delay(100);
    }
    // 1200 ms
    // Circle dim clockwise.
    for(int i = 2; i<14; i++){
      digitalWrite(i, HIGH);
      delay(100);
    }
    // 1200 ms
    // (200+1200+1200 = 2600)
    if(hour == 0){
      for(int i = 2; i<14; i++){
        digitalWrite(i, LOW);
        delay(100);//1200
      }
      // 1200 ms
      for(int i = 13; i>2; i--){
        digitalWrite(i, HIGH);
        delay(100);//1100
      }
      del = 100;
    }else{
      for(int i = 2; i<=hour+2; i++){
        digitalWrite(i, LOW);
        delay(100);
      }
      for(int i = hour+1; i>1; i--){
        digitalWrite(i, HIGH);
        delay(100);
      }
      del = 2400-(100*(hour+2))-(100*(hour-1));
    }
    // The end on the hourly animation
  }else{
    for(int i=2; i<14; i++){
      if(i!=hour+2 || i!=min+2){
        digitalWrite(i, HIGH);
      }
    }
    digitalWrite(hour+2, LOW);
    digitalWrite(min+2, LOW);
    // Blinking extra minutes (if any); If, say, the time is 13:27, minutes led will blink 2 times.
    for(int i = 0; i<exMin; i++){
      digitalWrite(min+2, LOW);
      delay(300);
      digitalWrite(min+2, HIGH);
      delay(200);
    }
    if (hour == min){
      digitalWrite(min+2, LOW); // Keeping hours lit after minutes blinking.
    }
    // I played with seconds blinking, and decided to hardcode two quick blinks.
    for(int i = 0; i<3; i++){
      digitalWrite(sec+2, LOW);
      delay(50);
      digitalWrite(sec+2, HIGH);
      delay(50);
      if(sec == hour){
        digitalWrite(sec+2, LOW); // Keep the hour led lit after blinking seconds.
      }
      if (sec == min && exMin == 0) {
        digitalWrite(min+2, LOW) // Keep the minutes led lit after blinking seconds if if is aliquot to 5 (minutes modulo 5 equals 0).
      }
    }
  }
  // If there was no change in Hours, set the delay between cycles to
  // 5 seconds minus the time for different animations
  if(!hrchange){
    del = 5000-(500*exMin)-300;
  }
  hr = hour;
  delay(del);
}
