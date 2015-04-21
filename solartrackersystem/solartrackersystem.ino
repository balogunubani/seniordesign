/*
MEM PR6B Senior Design Project: Solar Lighthouse
Group members: Ubani Balogun, Joseph Musembi, Ankit Patel, Steven Heidengren, Steve Forest
Arduino based Solar Tracker System Design
Author: Ubani Balogun

TODO: Describe Program code

*/

// Actuator definitions
const int AC_1[2] = {2,3};

// Indexes for Actuator wires
const int RED = 0;
const int BLACK = 1;

const int DELAY = 2000;
// setup
void setup(){
  // Setup Actuators
  pinMode(AC_1[RED],OUTPUT);
  pinMode(AC_1[BLACK],OUTPUT);
  
}


void loop(){
  extendActuator(AC_1);
  delay(DELAY);
  stopActuator(AC_1);
  delay(DELAY);
  retractActuator(AC_1);
  delay(DELAY);
  stopActuator(AC_1);
  delay(DELAY);
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


