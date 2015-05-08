/*
MEM PR6B Senior Design Project: Solar Lighthouse
Group members: Ubani Balogun, Joseph Musembi, Ankit Patel, Steven Heidengren, Steve Forest
Arduino based Solar Tracker System Design
Author: Ubani Balogun

TODO: Describe Program code

*/

#include <Servo.h>


// Actuator Definitions
int FRONT_AC_1[3] = {13,12,0};  // 1st wire, 2nd wire, height
int FRONT_AC_2[3] = {11,10,0};
int BACK_AC_1[3] = {9,8,0};
int BACK_AC_2[3] = {7,6,0};
// Consider substituting below values with calcualtions
const int AC_DISTANCE = 11;
const int AC_HALF_RANGE = 7;
const float S_A_RATIO = 12.0;
const float ANGLE_TO_HEIGHT = 0.2;
const float HEIGHT_TO_DELAY  = 2.0;

// Photoresistor definitions
const int photoR = A0;


// Indexes for Actuator wires and height
const int RED = 0, FALSE = 0, LEAD = 0;
const int BLACK = 1, TRUE = 1, FOLLOW = 1;
const int HEIGHT = 2;

// Common constants
const int DELAY = 2000;
const int DELAY_CALIB = 200;

// Servo Definitions
Servo myservo;
int servoPos = 0;
const int servoPin = 3;

//Dimmies
int incoming = 0;


// setup
void setup(){
  // Setup Actuators
  for (int pinId = 0; pinId < 2; pinId++){
    pinMode(FRONT_AC_1[pinId],OUTPUT);
    pinMode(FRONT_AC_2[pinId],OUTPUT);
    pinMode(BACK_AC_1[pinId],OUTPUT);
    pinMode(BACK_AC_2[pinId],OUTPUT);
  }
  
  // calibrateActuator(AC_1);
 
  // Setup Servo
  myservo.attach(servoPin);

  // Setup Serial communication for test
  Serial.begin(9600);
}


void loop(){
  //int sensorReading;
  //sensorReading = readSensor();
  Serial.println("======================NEW ROUND======================");
  while (Serial.available() == 0);
  if (Serial.available() > 0){
    incoming = Serial.parseInt();
    Serial.print("Ideal angle: "); Serial.println(incoming,DEC);
    adjustActuator(incoming);
    delay(2000);
  }
  Serial.println("ACTUATOR STATUS");
  Serial.print("FRONT AC: ");Serial.println(FRONT_AC_1[HEIGHT]);
  Serial.print("BACK AC: "); Serial.println(BACK_AC_1[HEIGHT]);
//  extendActuator(FRONT_AC_1);
//  extendActuator(FRONT_AC_2);
//  delay(80);
//  stopActuator(FRONT_AC_1);
//  stopActuator(FRONT_AC_2);
//  delay(1500);
//  extendActuator(BACK_AC_1);
//  extendActuator(BACK_AC_2);
//  delay(80);
//  stopActuator(BACK_AC_1);
//  stopActuator(BACK_AC_2);
//  delay(1500);
//  retractActuator(BACK_AC_1);
//  retractActuator(BACK_AC_2);
//  retractActuator(FRONT_AC_1);
//  retractActuator(FRONT_AC_2);
//  delay(80);
//  stopActuator(BACK_AC_1);
//  stopActuator(BACK_AC_2);
//  stopActuator(FRONT_AC_1);
//  stopActuator(FRONT_AC_2);
//  delay(1500);
}


// Extend the linear actuator
void extendActuator(const int actuator[]){
  digitalWrite(actuator[BLACK],LOW);
  delay(10);
  digitalWrite(actuator[RED],HIGH);
}


// Retract the linear actuator
void retractActuator(const int actuator[]){
  digitalWrite(actuator[RED],LOW);
  delay(10);
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

// Adjusts the linear actuators to the ideal position as indicated by the input
int adjustActuator(int ideal){
  /*
  * Calculate which side we are moving up by subtracting 90 from ideal and checking if the result is greater or lesser than 0.
  * Move opposing side down and update currentheight, if not already fully down
  * calculate the scaled angle adjustement from ideal by: scaled = ideal / ratio (12)
  * calculate the approx height difference from the angle using one of two methods:
    1. float newH = tan(scaled) * AC_DISTANCE (distance btw actuators). Note that tan uses radian here
    2. float newH = 0.2 * scaled. Gives a pretty close approximation. 
    Conclusion: 2nd method has been chosen
  * Store previous *_ACTUATOR_ID[HEIGHT] in temp variable, prevH
  * Store this calcuated height difference in the 2nd index of the ACTUATOR LIST.
  * Compute newH - prevH as diffH.
    if Value > 0
      Move actuator up by difference
    else
      Move actuator down by difference
  * Convert diffH to time value by diffH / HEIGHT to DELAY = timeH (seconds)
  * timeH by 1000 to convert to ms, and cast result to int. msTimeH
  * move actuator by msTimeH.
  */
  int side, prevH, newH, diffH;
  int *ac_set_up[2], *ac_set_down[2];
  side = (ideal - 90); 
  if (side > 0){
    prevH = FRONT_AC_1[HEIGHT];
    ac_set_up[LEAD] = FRONT_AC_1;
    ac_set_up[FOLLOW] = FRONT_AC_2;
    ac_set_down[LEAD] = BACK_AC_1;
    ac_set_down[FOLLOW] = BACK_AC_2;
    Serial.println("FRONT ACTUATORS TO MOVE UP");
  }else if(side < 0){
    prevH = BACK_AC_1[HEIGHT];
    ac_set_up[LEAD] = BACK_AC_1;
    ac_set_up[FOLLOW] = BACK_AC_2;
    ac_set_down[LEAD] = FRONT_AC_1;
    ac_set_down[FOLLOW] = FRONT_AC_2;
    Serial.println("BACK ACTUATORS TO MOVE UP");
  }else{
    retractActuator(BACK_AC_1);
    retractActuator(BACK_AC_2);
    retractActuator(FRONT_AC_1);
    retractActuator(FRONT_AC_2);
    delay(1500);
    BACK_AC_1[HEIGHT] = 0;
    BACK_AC_2[HEIGHT] = 0;
    FRONT_AC_1[HEIGHT] = 0;
    FRONT_AC_2[HEIGHT] = 0;
    return 0;
  }
  
  Serial.print("prevH: ");Serial.println(prevH);
  float scaled = ideal / S_A_RATIO;
  Serial.print("scaled: ");Serial.println(scaled);
  newH = (int) (ANGLE_TO_HEIGHT * 1000 * scaled / HEIGHT_TO_DELAY);
  Serial.print("NewH: ");Serial.println(newH);  
  diffH = newH - prevH;
  Serial.print("diffH before abs: ");Serial.println(diffH);
  if (diffH > 0){
    Serial.print("diffH: ");Serial.println(diffH);
//    move actuator up
    extendActuator(ac_set_up[LEAD]);
    extendActuator(ac_set_up[FOLLOW]);
    delay(diffH);
    stopActuator(ac_set_up[LEAD]);
    stopActuator(ac_set_up[FOLLOW]);
  }else{
//    move actuator down
    diffH = abs(diffH);
    retractActuator(ac_set_up[LEAD]);
    retractActuator(ac_set_up[FOLLOW]);
    delay(diffH);
    stopActuator(ac_set_up[LEAD]);
    stopActuator(ac_set_up[FOLLOW]);
  }
  // Set new Actuator height  
  ac_set_up[LEAD][HEIGHT] = newH;
  Serial.print("ac_set_up[LEAD][HEIGHT]: ");Serial.println(ac_set_up[LEAD][HEIGHT]);
  Serial.print("before downH: ");Serial.println(ac_set_down[LEAD][HEIGHT]);
  int downH = ac_set_down[LEAD][HEIGHT];
  if (downH != 0){
    retractActuator(ac_set_down[LEAD]);
    retractActuator(ac_set_down[FOLLOW]);
    delay(downH + 100);
    stopActuator(ac_set_down[LEAD]);
    stopActuator(ac_set_down[FOLLOW]);
    ac_set_down[LEAD][HEIGHT] = 0;  
    Serial.print("ac_set_down[LEAD][HEIGHT]: ");Serial.println(ac_set_down[LEAD][HEIGHT]);
  }
  return 0;
}
