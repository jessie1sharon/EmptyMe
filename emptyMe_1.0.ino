#include <DS3231.h>
#include <SoftwareSerial.h>

DS3231  rtc(SDA, SCL);
Time t;

const int OnHour = 10;  // clock settings
const int OnMin = 00;
const int OnHour1 = 22;
const int OnMin1 = 00;

int trigPin = 12;     // distance settings
int echoPin = 11;
int pingTravelTime ;
float pingTravelDistance;
float distanceToTarget;
int numMeas = 50;
float avMeas;
int j;
float bucket = 0;

void setup() {
pinMode(trigPin,OUTPUT);
pinMode(echoPin,INPUT);
Serial.begin(9600);
rtc.begin();

}

void loop() {

t = rtc.getTime();

if (t.hour == OnHour && t.min == OnMin){
  for (j=1;j<=numMeas;j=j+1){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(10);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    pingTravelTime = pulseIn(echoPin,HIGH);
    delay(1000);
    pingTravelDistance = (pingTravelTime*765.*5280.*12.*2.54)/(3600.*1000000);
    distanceToTarget = pingTravelDistance/2;
    bucket = bucket+distanceToTarget;
  }
  avMeas = bucket / numMeas;
  bucket = 0;
  
  if(avMeas < 3){  // if the measurement is less than 3 cm from the top i.e. full
  //Serial.print("Average Distance is ");
  //Serial.print(avMeas);
  //Serial.println(" cm");
  Serial.println("Recycling-Bin is full");
  }
  //else if(avMeas > 3){
   //   Serial.println("Recycling-Bin is not full");
  //}

  }
//else {
 // Serial.println(" it is not the time");
 // delay(1000);
 // }
}
