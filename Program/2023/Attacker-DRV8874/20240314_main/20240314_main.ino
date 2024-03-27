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
  motorSetup(10, 100, 100, 100, 100);

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

  // line
  refreshLine();
  
  if(lineBlock[4] != 0){
    digitalWrite(ledPin[1], 1);
    
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
      motorRaw(-100, 0, 100, 0, mpL);
      break;

    case 110:
      motorRaw(0, 100, 0, -100, mpL);
      break;

    case 11:
      motorRaw(100, 0, -100, 0, mpL);
      break;

    case 1001:
      motorRaw(0, -100, 0, 100, mpL);
      break;
    }
    delay(300);
    return;
  }
   
  digitalWrite(ledPin[1], 0);

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
  
  // ball
  refreshBall();

  if(ballSum < BALL_MIN){
    motorP(0, 0, 0, 0, 100);
    return;
  }

  switch(ballBig){
  case 0:
    motorP(100, 100, -100, -100, mpB);
    break;

  case 1:
  case 2:
  case 3:
    motorP(-100, 100, 100, -100, mpB);
    break;

  case 4:
  case 5:
    motorP(-100, 0, 100, 0, mpB);
    break;

  case 6:
  case 7:
  case 9:
  case 10:
    motorP(-100, -100, 100, 100, mpB);
    break;

  case 8:
    motorP(100, -100, -100, 100, mpB);
    break;

  case 11:
  case 12:
    motorP(0, -100, 0, 100, mpB);
    break;
  
  case 13:
  case 14:
  case 15:
    motorP(100, -100, -100, 100, mpB);
    break;
  }
}