#include "pin.h"
#include "motor.h"
#include "sensor.h"
#include "tone.h"
#include "camera.h"

Pixy2I2C pixy;

// Constants
// Dir
#define DIR_TOLERANCE 15
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
unsigned long startTime;
unsigned long startFront;
unsigned long currentTime;

void setup() {
  pinSetup();
  dirSetup();
  motorSetup(10, 100, 100, 100, 100);
  cameraSetup(pixy, 100);

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
  startTime = millis();
  start = startTime;
  startFront = startTime;
}

void loop() {

  // Camera
  refreshCamera(pixy);
  Serial.print(blockX);
  Serial.print("\t");
  Serial.println(blockWidth);

  if (digitalRead(btnPin[3]) == 0) {
    motorP(0, 0, 0, 0, 100);
    tone(bzPin, soundPitch[0]);
    tone(bzPin, soundPitch[3]);
    tone(bzPin, soundPitch[5]);
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



//-----------------------------------------------
/*
//dir-minute

  if(millis() - start > 500){
    while(true){
      refreshDir();
      if((-1 * DIR_TOLERANCE < dir && dir < DIR_TOLERANCE) == false){
        digitalWrite(ledPin[0], 1);
        mpDir = (dir * P_GAIN_DIR + diffDiff * D_GAIN_DIR) * (-1);
        motor(mpDir, mpDir, mpDir, mpDir, mpD);
        diffDiff = abs(dir) - abs(dirPrevious);
        dirPrevious = dir;
        start = millis();
        return;
      }
      digitalWrite(ledPin[0], 0);
      
    }
  }

*/



  // ========================================
 /* //かーめらかめらかーめらかめら　か！め！ら！
  if(blockX > 240 || blockWidth < 152){
      motorP(-100, 80, 90, -90, mpL);
      delay(200);
  }
  if(blockX < 78 || blockWidth < 152){
      motorP(90, -75, -85, 100, mpL);
      delay(200);
  }

*/

  // ========================================
  // Ball
  refreshBall16();
  int h = 0;
  if(ballSum < BALL_EXIST) {
    motorP(0, 0, 0, 0, mpB);
    return;
  }

  switch (ballBig) {
  case 0:
    motorP(90, 90, -90, -100, mpB);
    delay(400);
    if(digitalRead(A3) + digitalRead(A4) + digitalRead(A5) > digitalRead(A11) + digitalRead(12) + digitalRead(A13)){
      motorP(-100, 0, 100, 0, mpB);
      delay(400);
    }
    else{
      motorP(0, -100, 0, 100, mpB);
      delay(400);

    }
    
    
    
    
    /*if(digitalRead(A3) + digitalRead(A4) + digitalRead(A5) - digitalRead(A11) + digitalRead(12) + digitalRead(A13) >= 300){
      if(digitalRead(36) + digitalRead(37) + digitalRead(38) + digitalRead(39) + digitalRead(40) + digitalRead(41) == 0){
        motorP(-100, 0, 100, 0, mpB);
      }
      else{

      } 
  
    }
    if(digitalRead(11) + digitalRead(A12) + digitalRead(A13) - digitalRead(A3) + digitalRead(4) + digitalRead(A5) >= 300){
      if(digitalRead(36) + digitalRead(37) + digitalRead(38) + digitalRead(39) + digitalRead(40) + digitalRead(41) == 0){
        motorP(0, -100, 0, 100, mpB);
      }
      else{

      }
     
    }
    else{
      if(digitalRead(36) + digitalRead(37) + digitalRead(38) + digitalRead(39) + digitalRead(40) + digitalRead(41) == 0){
        motor(-100, -100, 100, 100, mpB);
      }

      break;
      
    }*/
      
  
  
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    motorP(-100, 95, 95, -100, mpB);
    tone(bzPin, soundPitch[0]);
    break;


  case 7:
  case 8:
  case 9:
    motorP(0, 0, 0, 0, mpB);
    tone(bzPin, soundPitch[11]);
    break;

  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    motorP(90, -75, -85, 100, mpB);
    tone(bzPin, soundPitch[4]);
    break;
  }  
  

//-----------------------------------------------------------------------------------

  // Dir
  


  refreshDir();
  if ((-1 * DIR_TOLERANCE < dir && dir < DIR_TOLERANCE) == false) {
    digitalWrite(ledPin[0], 1);
    mpDir = (dir * P_GAIN_DIR + diffDiff * D_GAIN_DIR) * (-1);
    motor(mpDir, mpDir, mpDir, mpDir, mpD);
    diffDiff = abs(dir) - abs(dirPrevious);
    dirPrevious = dir;
    start = millis();
    
    return;
  }

  digitalWrite(ledPin[0], 0);

  // ========================================

  

  

  //motorP(-100, 95, 95, -100, 100);



//-----------------------------------------------------------------




  // Line
  refreshLine();
  if(lineBlock[5] != 10) {
    digitalWrite(ledPin[1], 1);
    switch (lineBlock[5]) {
    case 11111:  //all
    case 10010:  //front_back in
    case 10100:  //front_back out
    case 1001:  //left_right
      motorP(0, 0, 0, 0, mpL);
      tone(bzPin, soundPitch[9]);
      break;


    case 10000:  //front
    case 10110:  //front_back out in
      motorP(75, 75, -75, -80, mpL);
      delay(350);
      break;

    case 1000:  //left
    case 1100:  //left_back out
    case 1110:  //left_back out in
    case 1010:  //left_back in
      if(blockX == 0 || blockWidth == 0){
        motorP(100, 10, -100, -10, mpL);
        delay(400);
        break;
      }
      else{
        motorP(0, 100, 0, -100, mpL);
        tone(bzPin, soundPitch[9]);
        break;
      }

      

    case 0:
      motorP(-80, -80, 80, 80, mpL);
      break;



    case 1:    //right
    case 11:   //back in_right
    case 111:  //back out in_right
    case 101:  //back out_right
      if(blockX == 0 || blockWidth == 0){
        motorP(10, 100, -10, -100, mpL);
        delay(400);
        break;
      }
      else{
        motorP(100, 0, -100, 0, mpL);
        tone(bzPin, soundPitch[9]);
        break;
      }
      

    case 11000:  //front_left
      motorP(95, 95, -95, -100, mpL);
      tone(bzPin, soundPitch[9]);
      break;


    case 10001:  //front_right
       motorP(95, 95, -95, -100, mpL);
       tone(bzPin, soundPitch[9]);
       break;



    case 110:   //back_in out
    case 100:   //back_out
    case 10:    //back_in
      if(blockX == 0 || blockWidth == 0){
        motorP(90, 90, -90, -100, mpL);
        delay(200);
        break;
      }
      else{
         motorP(-100, -100, 100, 100, mpL);
         break;
      }


      
     
    


       

    //default:
      //motorP(0, 0, 0, 0, mpL);
      //break;
    }
    return;
  }
    
  digitalWrite(ledPin[1], 0);



//---------------------------------------------------------------------------------------


  currentTime = millis();  
  if (currentTime - startTime >= 30000) {
    if(currentTime - startTime <= 31000){
      if(line[10] +line[11] + line[12] == 0){
          motorP(-100, -100, 100, 100, mpL);
          return;
       }
       else{
          motorP(100, 100, -100, -100, mpL);
          delay(200);
          
       }
    }
       
  }
  if (currentTime - startTime >= 60000) {
    if(currentTime - startTime <= 61000){
      if(line[10] +line[11] + line[12] == 0){
          motorP(-100, -100, 100, 100, mpL);
          return;
       }
       else{
          motorP(100, 100, -100, -100, mpL);
          delay(200);
          
       }
    }
       
  }
  if (currentTime - startTime >= 90000) {
    if(currentTime - startTime <= 91000){
      if(line[10] +line[11] + line[12] == 0){
          motorP(-100, -100, 100, 100, mpL);
          return;
       }
       else{
          motorP(100, 100, -100, -100, mpL);
          delay(200);
          
       }
    }
       
  }
  if (currentTime - startTime >= 120000) {
    if(currentTime - startTime <= 121000){
      if(line[10] +line[11] + line[12] == 0){
          motorP(-100, -100, 100, 100, mpL);
          return;
       }
       else{
          motorP(100, 100, -100, -100, mpL);
          delay(200);
          
       }
    }
       
  }


/*

motorP(0, 0, 0, 0, mpB);
    if(A0 >= 713){
      motorP(90, 90, -90, -100, mpB);
      delay(500);
      if(A0 + A1 + A15 > A4 + A5 + A6){
        if(A0 + A1 + A15 > A10 + A11 + A12){
          motorP(90, 90, -90, -100, mpB);
          delay(500);
        }
        else{
          motorP(-70, 95, 65, -100, mpB);
          delay(500);
        }

      }
      else{
        motorP(-100, 65, 95, -70, mpB);
      }
  
      

    
  
    }




*/




  
}