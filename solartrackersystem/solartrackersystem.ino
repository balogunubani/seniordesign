/*
MEM PR6B Senior Design Project: Solar Lighthouse
Group members: Ubani Balogun, Joseph Musembi, Ankit Patel, Steven Heidengren, Steve Forest
Arduino based Solar Tracker System Design
Author: Ubani Balogun

TODO: Describe Program code

*/

#include <Servo.h>


// Actuator Definitions
const int AC_1[2] = {2,3};

// Photoresistor definitions
const int photoR = A0;


// Indexes for Actuator wires
const int RED = 0;
const int BLACK = 1;

// Common constants
const int DELAY = 2000;
const int DELAY_CALIB = 200;

// Servo Definitions
Servo myservo;
int servoPos = 0;
const int servoPin = 9;

// setup
void setup(){
  // Setup Actuators
  pinMode(AC_1[RED],OUTPUT);
  pinMode(AC_1[BLACK],OUTPUT);
  
  // calibrateActuator(AC_1);
 
  // Setup Servo
  myservo.attach(servoPin);

  // Setup Serial communication for test
  Serial.begin(9600);
}


void loop(){
  int sensorReading;
  sensorReading = readSensor();
  Serial.print("Position with Max Intensity: "); Serial.println(sensorReading);
  delay(1000);
}


// Extend the linear actuator
void extendActuator(const int actuator[]){
  digitalWrite(actuator[BLACK],LOW);
  delay(100);
  digitalWrite(actuator[RED],HIGH);
}


// Retract the linear actuator
void retractActuator(const int actuator[]){
  digitalWrite(actuator[RED],LOW);
  delay(100);
  digitalWrite(actuator[BLACK],HIGH);
}

// Stop linear actuator
void stopActuator(const int actuator[]){
  digitalWrite(actuator[RED],LOW);
  digitalWrite(actuator[BLACK],LOW);
}

// Determines the distance Actuator moves in a second
void calibrateActuator(const int actuator[]){
  retractActuator(actuator);
  delay(5000);
  stopActuator(actuator);
  extendActuator(actuator);
  delay(DELAY_CALIB);
  stopActuator(actuator);
}

// Sweeps the sensor 180 degrees, reading light intensities and returns the angle of the highest intensity
int readSensor(){
  int intens = 0;
  int maxPos = 0;
  int finalPos = 0;
  for (servoPos = 0; servoPos < 180; servoPos+= 10){
    myservo.write(servoPos);
    delay(500);
    
    if (analogRead(photoR) > intens){
      intens = analogRead(photoR);
      // Serial.print("Intens:");Serial.println(intens);
      finalPos = servoPos;
      // Serial.print("Intens:");Serial.print(intens);Serial.print(" finalPos:");Serial.println(finalPos);
    }
  }
  myservo.write(finalPos);
  return finalPos;
}
