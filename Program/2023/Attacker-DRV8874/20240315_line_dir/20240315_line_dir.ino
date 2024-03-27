#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

// Constants
#define DIR_TOLERANCE 10
#define P_GAIN_DIR 0.95
#define D_GAIN_DIR 0.7

#define BALL_MIN 150

// Variables
float dirPrevious = 0;
float diffDiff = 0;

int mpD = 80;
int mpL = 100;
int mpB = 80;

float mpDir;
float mpLine;
float mpBall;

void setup() {
  pinSetup();
  dirSetup();
  motorSetup(10, 100, 95, 100, 95);

  while (true) {
    refreshDirRaw();

    if(dir >= 9000){
      tone(bzPin, 440);
      digitalWrite(ledPin[2], 1);
    }
    else{
      noTone(bzPin);
      digitalWrite(ledPin[2], 0);
      if(digitalRead(btnPin[0]) == 1){
        dirOpponent = dir;
        dirCalibration = true;
        waitBtn(0);
        break;
      }
    }

    if(digitalRead(btnPin[0]) == 1 && digitalRead(btnPin[1]) == 1){
      dirCalibration = true;
      refreshDirRaw();
      dirOpponent = dir;
      waitBtn(0);
      noTone(bzPin);
      break;
    }
  }
}

void loop() {
  // check motor lever
  if(digitalRead(btnPin[3]) == 1){
    motorP(0, 0, 0, 0, 100);
    return;
  }

  // dir
  refreshDir();

  if((-1*DIR_TOLERANCE < dir && dir < DIR_TOLERANCE) == false){
    digitalWrite(ledPin[0], 1);
    mpDir = (dir * P_GAIN_DIR + diffDiff * D_GAIN_DIR) * (-1);
    motorRaw(mpDir, mpDir, mpDir, mpDir, mpD);
    diffDiff = abs(dir) - abs(dirPrevious);
    dirPrevious = dir;
    return;
  }

  digitalWrite(ledPin[0], 0);

  // line
  refreshLine();
  
  if(lineBlock[4] != 0){
    digitalWrite(ledPin[1], 1);

    motorRaw(0, 0, 0, 0, 100);
    
    switch(lineBlock[4]){
    case 1111:
    case 1010:
    case 101:
      motorP(0, 0, 0, 0, mpL);
      break;

    case 1000:
    case 1101:
      motorRaw(-100, -100, 100, 100, mpL);
      break;

    case 100:
    case 1110:
      motorRaw(100, -100, -100, 100, mpL);
      break;

    case 10:
    case 111:
      motorRaw(100, 100, -100, -100, mpL);
      break;

    case 1:
    case 1011:
      motorRaw(-100, 100, 100, -100, mpL);
      break;

    case 1100:
      motorRaw(0, -100, 0, 100, mpL);
      break;

    case 110:
      motorRaw(100, 0, -100, 0, mpL);
      break;

    case 11:
      motorRaw(0, 100, 0, -100, mpL);
      break;

    case 1001:
      motorRaw(-100, 0, 100, 0, mpL);
      break;
    }
    delay(200);
    return;
  }

  motorP(-100, 100, 100, -100, mpB);
}