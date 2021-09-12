#include <Servo.h>
#include "RTClib.h"

#define RED_LED 8
#define GREEN_LED 7
#define BLUE_LED 6
#define VIBRATION_MOTOR 5
#define DETECTOR_PIN 4

Servo myservo;   
RTC_Millis rtc;

int pos = 0;
int feedTime = 0;
int cnt = 1;
int mins = 0;
int num_cats = 2;
int quarter_cup = 5 * num_cats;  // 10 is a quarter of a cup 
bool dispensing = true;
bool startup = true;

void setup() {
  ///Serial.begin(9600);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2021, 9, 11, 22, 30, 0));  //set (year, month, day, hour, minutes, seconds)
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(DETECTOR_PIN, INPUT);
  pinMode(VIBRATION_MOTOR,OUTPUT);
  myservo.attach(9);
  myservo.detach();
}

void loop() {
  runningLED();
  DateTime now = rtc.now();
  // Dispense food every 6 hours starting at 6
  for(int i = 0;i <= 4;i++){
    if(now.hour() == 6 * i + feedTime && now.minute() == mins || startup == true){
      dispensing = true;
      startup = false;
      }
  }
  while(dispensing == true){
    dispenseLED();
    getFood();
    delay(2000);
    while(jamDetect() == true){
      unjamMotor(); 
      jamDetected_LED();
      delay(3000);
    }
    
    if(jamDetect() == false)
      dispenseFood();
      
    if(cnt == quarter_cup){
      cnt = 0;
      dispensing = false;
    }
    cnt++;
  }
  delay(1000);
}
int jamDetect(){
  int val = digitalRead(DETECTOR_PIN);
  return val;
}
// Dispensing blue LED turns on when digital pin 6 is off
void runningLED(){
  digitalWrite(GREEN_LED,HIGH);
  delay(1000);
  digitalWrite(GREEN_LED,LOW);
  delay(1000);
}
void dispenseLED(){
  digitalWrite(BLUE_LED,HIGH);
  delay(1000);
  digitalWrite(BLUE_LED,LOW);
  delay(1000);
}
void jamDetected_LED(){
  digitalWrite(RED_LED,HIGH);
  delay(1000);
  digitalWrite(RED_LED,LOW);
  delay(1000);
}
// Turn on vibration motor to unjam tube
void unjamMotor(){
  digitalWrite(VIBRATION_MOTOR,HIGH);
  delay(3000);
  digitalWrite(VIBRATION_MOTOR,LOW);
  delay(1000);
}
// Dispense food from feed disc
void dispenseFood(){
  myservo.attach(9);
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);              
    delay(25);                       
  }
  delay(1000);
  myservo.detach();
}
// Recieve food from feed tube
void getFood(){ 
  myservo.attach(9);
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);           
    delay(25);                    
  }  
  delay(1000);
  myservo.detach();
}
