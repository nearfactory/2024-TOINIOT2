#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

#include "camera.h"

// Constants
PixyI2C pixy(0x54);

#define LEFT_LIM 110
#define RIGHT_LIM 210

// Variables
short int lastGoal = 0;

void setup() {
  pinSetup();
  cameraSetup(pixy, 100);
  tone(bzPin, 440);
  digitalWrite(ledPin[2], 1);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  refreshCamera(pixy);
  if(blockNum != 0 && blockX < LEFT_LIM){
    lastGoal = 1;
  }
  else if(blockNum != 0 && blockX > RIGHT_LIM){
    lastGoal = 2;
  }
  else if(blockNum != 0){
    lastGoal = 0;
  }

  Serial.print(lastGoal);
  Serial.print("\t");
  Serial.print(blockX);
  Serial.print("\t");
  Serial.println(blockWidth);

  switch(lastGoal){
  case 0:
    digitalWrite(ledPin[0], 0);
    digitalWrite(ledPin[1], 0);
    break;
    
  case 1:
    digitalWrite(ledPin[0], 1);
    digitalWrite(ledPin[1], 0);
    break;
    
  case 2:
    digitalWrite(ledPin[0], 0);
    digitalWrite(ledPin[1], 1);
    break;
    
  default:
    digitalWrite(ledPin[0], 1);
    digitalWrite(ledPin[1], 1);
    break;    
  }

  delay(40);
}