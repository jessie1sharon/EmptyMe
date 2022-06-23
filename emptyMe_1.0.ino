#include <DS3231.h>
#include <SoftwareSerial.h>

DS3231  rtc(SDA, SCL);
Time t;

SoftwareSerial sensor(12,11);
SoftwareSerial blue(2,3);

const int OnHour = 05;  // clock settings
const int OnMin = 00;
const int OnHour1 = 17;
const int OnMin1 = 00;

int state = 0;  //state for blue

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
pinMode(trigPin,OUTPUT);  // input to distance sensor
pinMode(echoPin,INPUT);   // output from distance sensor
sensor.begin(9600);
blue.begin(9600);
blue.println("Starting...");

rtc.begin();

}

void checkState(){
  if (blue.available()> 0){
    state = blue.read(); // Reads the data from the bluetooth
}
if (state == 'R'){
    String StrUno = "average measure is ";  // send last average
    String StrDos = String(StrUno + avMeas);
    blue.println(StrDos);
    state = 0;
 }
}

void loop() {
t = rtc.getTime();

if ((t.hour == OnHour && t.min == OnMin) || (t.hour == OnHour1 && t.min == OnMin1)){   // in certain times check if full
  for (j=1;j<=numMeas;j=j+1){
    digitalWrite(trigPin,LOW);
    delayMicroseconds(10);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    pingTravelTime = pulseIn(echoPin,HIGH);
    delay(1200);      // if full print once
    pingTravelDistance = (pingTravelTime*765.*5280.*12.*2.54)/(3600.*1000000);  // measure distance in cm
    distanceToTarget = pingTravelDistance/2;
    bucket = bucket+distanceToTarget;   // measure multiple times to take the average distance
    checkState();
  }
  avMeas = bucket / numMeas;
  bucket = 0;   // for next measure
  
  if(avMeas < 10){    // if distance smaller than 10 cm - its full
  //sensor.print("Average Distance is ");
  //sensor.print(avMeas);
  //sensor.println(" cm");
  //sensor.println("Recycling-Bin is full");
    blue.println("Recycling-Bin No.1 is full");  // send the app that is full
  }
  //else if(avMeas > 10){
   //   sensor.println("Recycling-Bin is not full");
  //}

  }
//else {
 // sensor.println(" it is not the time");
 // blue.println(" it is not the time");

 // delay(1000);
 // }
 checkState();

}
