/*
MEM Senior Design PR6B
@author: Ubani Balogun
Group members: Ubani Balogun, Joseph Musembi, Steven Heidengren, Steven Forrest, Ankit Patel

Test for the photo resistor sensor used for solar tracking
Reference: http:/playground.arduino.cc/Learning/PhotoResistor
Circuit:
----------------------------------------------------

           PhotoR     10K
 +5    o---/\/\/--.--/\/\/---o GND
                  |
 Pin A0 o----------

----------------------------------------------------
*/
#include <stdarg.h>

int photoResistor = A0;

void setup(){
  Serial.begin(9600);
}


void loop(){
  Serial.println(analogRead(photoResistor));
  delay(1000);
}


// Helper Functions
void printf(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}
