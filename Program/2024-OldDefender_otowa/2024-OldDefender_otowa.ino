#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"
#include "camera.h"

Pixy2I2C pixy;

// Constants
// Dir
#define DIR_TOLERANCE 10
#define P_GAIN_DIR 0.95
#define D_GAIN_DIR 0.7

// Ball
#define BALL_EXIST 100

// Variables

// Dir
float dirPrevious = 0;
float diffDiff = 0;
float mpDir;

// MotorPower
int mpD = 100;
int mpL = 80;
int mpB = 100;

// Timer
unsigned long start;

void setup() {
  pinSetup();
  dirSetup();
  motorSetup(10, 100, 100, 100, 100);
  // cameraSetup(pixy, 100);

  digitalWrite(ledPin[0], 1);

  while (true) {
    refreshDirRaw();
    if (dir > 9000) {
      tone(bzPin, soundPitch[9]);
    } else {
      noTone(bzPin);
      dirCalibration = true;
    }

    if (digitalRead(btnPin[0]) == 1) {
      waitBtn(0);
      dirOpponent = dir;
      break;
    }
  }

  digitalWrite(ledPin[0], 0);
  start = millis();
}

void loop() {

  // Camera
  refreshCamera(pixy);
  Serial.print(blockX);
  Serial.print("\t");
  Serial.println(blockWidth);

  if (digitalRead(btnPin[3]) == 1) {
    motorP(0, 0, 0, 0, 100);
    return;
  }

  else{
    noTone(bzPin);
    int c = digitalRead(8);
    int d = digitalRead(9);
    if (c == 1) {
       motorP(95, 95, -95, -100, mpD);
       delay(2000);
    }
    if(d == 1){
      motorP(95, 95, -95, -100, mpD);
      delay(2000);
    }
  }


 
  // ========================================

  // Line
  refreshLine();
  if(lineBlock[7] != 0) {
    digitalWrite(ledPin[1], 1);
    switch (lineBlock[7]) {
    //case 1111111:  //all
    //case 1000100:  //front_back in
    //case 1001000:  //front_back out
    //case 10001:  //left in_right in
    //case 100010: //left out_right out
    //case 110011: //left out in_right out in
      //motorP(0, 0, 0, 0, mpL);
      //tone(bzPin, soundPitch[9]);
      //break;


    case 1000000:  //front
    //case 1001100:  //front_back out in
      motorP(100, 100, -85, -85, mpL);
      delay(20);
      break;
    
    case 10000:   //left in
    case 100000:  //left out
    case 101000:  //left out_back out
    case 101100:  //left out_back out in
    case 100100:  //left out_back in
      if(blockNum == 0){
        //motorP(-100, 100, 100, -100, mpL);
        motorP(0, 100, 0, -100, mpL);
        delay(400);
        motorP(0, 0, 0, 0, mpL);
        delay(50);
        motorP(-100, -100, 80, 80, mpL);
        delay(100);
        break;
      }
      else{
        //motorP(100, -80, -100, 80, mpL);
        motorP(100, 0, -100, 0, mpL);
        delay(200);
        tone(bzPin, soundPitch[9]);
        break;
      }

      

    //case 0:
    //  motorP(-80, -80, 80, 80, mpL);
    //  break;


    
    case 1:     //right in
    case 10:    //right out
    case 110:   //back in_right out
    case 1110:  //back out in_right out
    case 1010:  //back out_right out
      if(blockNum == 0){
        //motorP(100, -80, -100, 80, mpL);
        motorP(100, 0, -100, 0, mpL);
        delay(400);
        motorP(0, 0, 0, 0, mpL);
        delay(50);
        motorP(-100, -100, 80, 80, mpL);
        delay(100);
        break;
      }
      else{
        //motorP(-100, 100, 100, -100, mpL);
        motorP(0, 100, 0, -100, mpL);
        delay(200);
        tone(bzPin, soundPitch[9]);
        break;
      }
      

    //case 1100000:  //front_left out
    //  motorP(95, 95, -95, -100, mpL);
    //  tone(bzPin, soundPitch[9]);
    //  break;


    //case 1000010:  //front_right out
    //   motorP(95, 95, -95, -100, mpL);
    //   tone(bzPin, soundPitch[9]);
    //   break;



    case 1100:   //back_in out
    case 1000:   //back_out
    case 100:    //back_in
      if(blockNum == 0){
        motorP(100, 100, -80, -80, mpL);
        delay(200);
        break;
      }
      else{
         motorP(100, 100, -80, -80, mpL); 
         break;
      }

    default:
      motorP(0, 0, 0, 0, mpL);
      break;
    }
    return;
  }


  digitalWrite(ledPin[1], 0);

  //------------------------------------------------------------------

  // Dir
  while(true){
    refreshDir();
    if(-1 * DIR_TOLERANCE < dir && dir < DIR_TOLERANCE){
      break;
    }
    digitalWrite(ledPin[0], 1);
    mpDir = (dir * P_GAIN_DIR + diffDiff * D_GAIN_DIR) * (-1);
    motor(mpDir, mpDir, mpDir, mpDir, mpD);
    diffDiff = abs(dir) - abs(dirPrevious);
    dirPrevious = dir;
    start = millis();
  }

  digitalWrite(ledPin[0], 0);



  

  // ========================================
 

  // Ball
  refreshBall16();
  // if(ballSum < BALL_EXIST) {
  //   motorP(0, 0, 0, 0, mpB);
  //   return;
  // }

  switch (ballBig) {
  case 0:
    motorP(0, 0, 0, 0, mpB);
    break;

  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    motorP(35, -100, -35, 100, mpB);
    tone(bzPin, soundPitch[0]);
    break;


  
  case 8:
  
    motorP(0, 0, 0, 0, mpB);
    tone(bzPin, soundPitch[11]);
    break;

  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    motorP(-100, 50, 100, -50, mpB);
    tone(bzPin, soundPitch[4]);
    break;
  }

  
    
  
}