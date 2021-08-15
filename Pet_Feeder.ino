#include <Servo.h>
#include "RTClib.h"

#define greenLED 7
#define blueLED 6
#define redLED 8

Servo myservo;   
RTC_Millis rtc;

int pos = 0;
int firstTime = 0;
int cnt = 1;
int mins = 0;

bool dispensing = true;

void setup() {
  ///Serial.begin(9600);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2021, 8, 15, 15, 5, 0));  //set (year, month, day, hour, minutes, seconds)
  pinMode(greenLED,OUTPUT);
  pinMode(blueLED,OUTPUT);
  pinMode(redLED,OUTPUT);
  myservo.attach(9);
  myservo.detach();
}

void loop() {
  runningLED();
  DateTime now = rtc.now();
  for(int i = 0;i <= 4;i++){
    if(now.hour() == 6 * i + firstTime && now.minute() == mins){
    ///Serial.println("dispensing food");
      dispensing = true;
    }
  }
  while(dispensing == true){
    getFood();
    dispenseLED();
    if(cnt == 10){
      cnt = 0;
      dispensing = false;
    }
    cnt++;
  }
  delay(1000);
}

void runningLED(){
  digitalWrite(redLED,HIGH);
  delay(1000);
  digitalWrite(redLED,LOW);
  delay(1000);
}
void dispenseLED(){
  digitalWrite(dispenseLED,HIGH);
  delay(500);
  digitalWrite(dispenseLED,LOW);
}
void getFood(){
  myservo.attach(9);
  // recieve food from feeder
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);           
    delay(25);                    
  }
  delay(1000);
  // dispense food from feed disc
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);              
    delay(25);                       
  }
  delay(1000);
  myservo.detach();
}
