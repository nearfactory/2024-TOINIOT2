#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"

#include "camera.h"

// Constants
#define DIR_TOLERANCE 10
#define P_GAIN_DIR 0.95
#define D_GAIN_DIR 0.7

#define BALL_MIN 100
#define BALL_FAR 1000
#define MPB_GAIN 0.85

#define LINEF_COUNT 3
#define LINEL_COUNT 3
#define LINEB_COUNT 3
#define LINER_COUNT 3

#define FRONTTURN_TOLERANCE 1000
#define FRONTTURN_TIME 125

#define LEFT_LIM 110
#define RIGHT_LIM 210

PixyI2C pixy(0x54);

// Variables
float dirPrevious = 0;
float diffDiff = 0;

int mpD = 80;
int mpL = 100;
int mpB = 100;

short int justBefore = 0;
short int lineF = 0;
short int lineL = 0;
short int lineB = 0;
short int lineR = 0;

unsigned long frontStart;
bool frontFlag = false;

short int lastGoal = 0;

float mpDir;
float mpLine;
float mpBall;

void setup() {
  pinSetup();
  dirSetup();
  cameraSetup(pixy, 100);
  motorSetup(10, 100, 100, 90, 90);

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
    
    switch(lineBlock[4]){
    case 1111:
      motorP(0, 0, 0, 0, mpL);
      break;

    case 101:
      if(justBefore == 1 || justBefore == 2 || justBefore == 3){
        motorP(100, -100, -100, 100, mpL);
        break;
      }
      else if(justBefore == 5 || justBefore == 6 || justBefore == 7){
        motorP(-100, 100, 100, -100, mpL);
        break;
      }
      else{
        motorP(0, 0, 0, 0, mpL);
        break;
      }

    case 1010:
      if(justBefore == 0 || justBefore == 1 || justBefore == 7){
        motorP(-100, -100, 100, 100, mpL);
        break;
      }
      else if(justBefore == 4 || justBefore == 3 || justBefore == 5){
        motorP(100, 100, -100, -100, mpL);
        break;
      }
      else{
        motorP(0, 0, 0, 0, mpL);
        break;
      }

    case 1000:
    case 1101:
      motorP(-100, -100, 100, 100, mpL);
      if(lineF > LINEF_COUNT){
        delay(150);
        lineF = 0;
        lineL = 0;
        lineB = 0;
        lineR = 0;
        justBefore = 4;
        break;
      }
      justBefore = 4;
      lineF += 1;
      lineL = 0;
      lineB = 0;
      lineR = 0;
      break;

    case 100:
    case 1110:
      motorP(100, -100, -100, 100, mpL);
      if(lineL > LINEL_COUNT){
        delay(150);
        lineF = 0;
        lineL = 0;
        lineB = 0;
        lineR = 0;
        justBefore = 6;
        break;
      }
      justBefore = 6;
      lineF = 0;
      lineL += 1;
      lineB = 0;
      lineR = 0;
      break;

    case 10:
    case 111:
      motorP(100, 100, -100, -100, mpL);
      if(lineB > LINEB_COUNT){
        delay(150);
        lineF = 0;
        lineL = 0;
        lineB = 0;
        lineR = 0;
        justBefore = 0;
        break;
      }
      justBefore = 0;
      lineF = 0;
      lineL = 0;
      lineB += 1;
      lineR = 0;
      break;

    case 1:
    case 1011:
      motorP(-100, 100, 100, -100, mpL);
      if(lineR > LINER_COUNT){
        delay(150);
        lineF = 0;
        lineL = 0;
        lineB = 0;
        lineR = 0;
        justBefore = 2;
        break;
      }
      justBefore = 2;
      lineF = 0;
      lineL = 0;
      lineB = 0;
      lineR += 1;
      break;

    case 1100:
      motorP(0, -100, 0, 100, mpL);
      justBefore = 5;
      break;

    case 110:
      motorP(100, 0, -100, 0, mpL);
      justBefore = 7;
      break;

    case 11:
      motorP(0, 100, 0, -100, mpL);
      justBefore = 1;
      break;

    case 1001:
      motorP(-100, 0, 100, 0, mpL);
      justBefore = 3;
      break;
    }
    return;
  }

  digitalWrite(ledPin[1], 0);
  
  // ball
  refreshBall();

  if(ballSum < BALL_MIN){
    motorP(0, 0, 0, 0, 100);
    frontFlag = false;
    return;
  }

  if(ballSum < BALL_FAR){
    // ボールは遠い
    // tone(bzPin, soundPitch[11]);
    switch(ballBig){
    case 0:
    case 1:
    case 15:
      motorP(100, 100, -100, -100, mpB);
      justBefore = 0;
      frontFlag = false;
      break;

    case 2:
      motorP(0, 100, 0, -100, mpB);
      justBefore = 1;
      frontFlag = false;
      break;

    case 3:
    case 4:
      motorP(-100, 100, 100, -100, mpB);
      justBefore = 2;
      frontFlag = false;
      break;

    case 5:
    case 6:
      motorP(-100, 0, 100, 0, mpB);
      justBefore = 3;
      frontFlag = false;
      break;

    case 7:
    case 9:
      motorP(-100, -100, 100, 100, mpB);
      justBefore = 4;
      frontFlag = false;
      break;

    case 8:
      motorP(0, -100, 0, 100, mpB);
      justBefore = 5;
      frontFlag = false;
      break;

    case 10:
    case 11:
      motorP(0, -100, 0, 100, mpB);
      justBefore = 5;
      frontFlag = false;
      break;

    case 12:
    case 13:
      motorP(100, -100, -100, 100, mpB);
      justBefore = 6;
      frontFlag = false;
      break;

    case 14:
      motorP(100, 0, -100, 0, mpB);
      justBefore = 7;
      frontFlag = false;
      break;
    }
  }
  else{
    // ボールは近い
    // tone(bzPin, soundPitch[7]);
    switch(ballBig){
    case 0:
      switch(lastGoal){
      case 0:
        motorP(100, 100, -100, -100, mpB*MPB_GAIN);
        // digitalWrite(ledPin[0], 0);
        // digitalWrite(ledPin[1], 0);
        break;

      case 1:
        motorP(100, 100, -100, -100, mpB*MPB_GAIN);
        motorP(0, 100, 0, -100, mpB*MPB_GAIN);
        // digitalWrite(ledPin[0], 0);
        // digitalWrite(ledPin[1], 1);
        break;

      case 2:
        motorP(100, 100, -100, -100, mpB*MPB_GAIN);
        motorP(100, 0, -100, 0, mpB*MPB_GAIN);
        // digitalWrite(ledPin[0], 1);
        // digitalWrite(ledPin[1], 0);
        break;
      }
      
      if(frontFlag == false){
        frontFlag = true;
      frontStart = millis();
      }
      if(frontFlag == true && millis() - frontStart > FRONTTURN_TOLERANCE){
        motorP(100, 100, 100, 100, mpB);
        delay(FRONTTURN_TIME);
        frontFlag = false;;
      }
      
      justBefore = 0;
      break;

    case 1:
    case 2:
    case 3:
      motorP(-100, 100, 100, -100, mpB*MPB_GAIN);
      justBefore = 2;
      frontFlag = false;
      break;

    case 4:
      motorP(-100, 0, 100, 0, mpB*MPB_GAIN);
      justBefore = 3;
      frontFlag = false;
      break;

    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
      motorP(-100, -100, 100, 100, mpB*MPB_GAIN);
      justBefore = 4;
      frontFlag = false;
      break;

    case 8:
      motorP(100, -100, -100, 100, mpB*MPB_GAIN);
      justBefore = 2;
      frontFlag = false;
      break;

    case 12:
      motorP(0, -100, 0, 100, mpB*MPB_GAIN);
      justBefore = 5;
      frontFlag = false;
      break;
    
    case 13:
    case 14:
    case 15:
      motorP(100, -100, -100, 100, mpB*MPB_GAIN);
      justBefore = 6;
      frontFlag = false;
      break;
    }
  }

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
}