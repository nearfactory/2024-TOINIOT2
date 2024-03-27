#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

// Constants
#define DIR_TOLERANCE 6
#define P_GAIN_DIR 0.95
#define D_GAIN_DIR 0.8

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
  motorSetup(10, 100, 100, 100, 100);

  while (true) {
    refreshDirRaw();
    Serial.println(dir);

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
  }

  // dir
  while(true){
    refreshDir();

    if(-1*DIR_TOLERANCE < dir && dir < DIR_TOLERANCE){
      digitalWrite(ledPin[0], 0);
      break;
    }

    digitalWrite(ledPin[0], 1);
    mpDir = (dir * P_GAIN_DIR + diffDiff * D_GAIN_DIR) * (-1);
    motorRaw(mpDir, mpDir, mpDir, mpDir, mpD);
    diffDiff = abs(dir) - abs(dirPrevious);
    dirPrevious = dir;
  }

  motorRaw(0, 0, 0, 0, 100);
}