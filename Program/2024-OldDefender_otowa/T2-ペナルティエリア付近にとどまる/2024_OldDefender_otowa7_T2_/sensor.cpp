// Arduino Include
#include <Arduino.h>

// Main Include
#include "sensor.h"
#include "pin.h"

// Other Include
#include <math.h>

// Include for DirSensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// DirSensor
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

// Variable
float dir;
float dirPrev;
float dirOpponent;
bool dirCalibration = false;

unsigned short int line[22];
unsigned int lineBlock[6];

unsigned short int ball[16];
float ballX;
float ballY;
float ballAngle;
int ballSum;
short int ballBig;

// Function
// Dir ==============================================
void dirSetup(void) {
    if (!bno.begin())
    {
        Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    bno.setExtCrystalUse(true);
    Serial.println("BNO055 Connected Successfully!");

    return;
}

void refreshDirRaw(void) {
    // Calibration Check
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    
    if (gyro != 3 || mag != 3 && dirCalibration == false) {
        // Serial.println("Calibration hasn't finished yet...");
        dir = 9900 + gyro * 10 + mag * 1;
        return;
    }
    

    // Euler Angle
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    imu::Quaternion quat = bno.getQuat();

    /*
    // Quarternion Angle (Needed to be calculated into YAW angle)
    Serial.print("qW: ");
    Serial.print(quat.w(), 4);
    Serial.print(" qX: ");
    Serial.print(quat.x(), 4);
    Serial.print(" qY: ");
    Serial.print(quat.y(), 4);
    Serial.print(" qZ: ");
    Serial.print(quat.z(), 4);
    Serial.print("\t\t");
    */

    // Refresh Value
    dirPrev= dir;
    dir = euler.x();
    return;
}

void refreshDir(void) {
    refreshDirRaw();
    dir -= dirOpponent;
    dir = dir < -180 ? dir+360 : dir;
    dir = dir > 180 ? dir-360 : dir;
    return;
}

// Line =============================================
void refreshLine(void) {
  for (int i=0; i<22; i++) {
      line[i] = digitalRead(linePin[i]);
  }

  //front
  if(line[0] + line[1] + line[2] + line[3] == 0){
      lineBlock[0] = 0;
  }
  else{
      lineBlock[0] = 1;
  }
  
  if(line[4] + line[5] + line[6] == 0){
    lineBlock[1] = 0;
  }
  else{
    lineBlock[1] = 1;
  }

  if(line[7] + line[8] + line[9] == 0){
    lineBlock[2] = 0;
  }
  else{
    lineBlock[2] = 1;
  }

  if(line[10] + line[12] == 0){
    lineBlock[3] = 0;
  }
  else{
    lineBlock[3] = 1;
  }
  

  if(line[13] + line[15] == 0){
    lineBlock[4] = 0;
  }
  else{
    lineBlock[4] = 1;
  }

  if(line[16] + line[17] + line[18] == 0){
    lineBlock[5] = 0;
  }
  else{
    lineBlock[5] = 1;
  }

  if(line[19] + line[21] == 0){
    lineBlock[6] = 0;
  }
  else{
    lineBlock[6] = 1;
  }
  //                     front              left_out               left_in            back_out           back_in            right_out         right_in
  lineBlock[7] = lineBlock[0]*1000000 + lineBlock[1]*100000 + lineBlock[2]*10000 + lineBlock[3]*1000 + lineBlock[4]*100 + lineBlock[5]*10 + lineBlock[6];
  return;
}

void refreshLinePrint(void) {
    refreshLine();

    for (int i = 0; i < 22; i++) {
        Serial.print(line[i]);
        Serial.print(",");
    }
    Serial.println();
    return;
}

// Ball =============================================

void ballEdit(void){
 
    ball[0] += 0;
    ball[1] += 0;
    ball[2] += 0;
    ball[4] += 0;
    ball[5] += 0;
    ball[6] += 50;
    ball[7] += -50;
    ball[8] += 0;
    ball[9] += 0;
    ball[11] += 0;
    ball[12] += 0;
    ball[13] += 50;
    ball[14] += 0;
    ball[15] += 0;
}

void refreshBall8(void) {
    ballX = 0;
    ballY = 0;
    for (int i = 0; i < 8; i++) {
        ball[i*2] = 1023 - analogRead(ballPin[i*2]);
        if(ball[i*2] >= 300){
            ballX += ball[i*2] * cos(radians(22.5*i*2 + 90));
            ballY += ball[i*2] * sin(radians(22.5*i*2 + 90));
        }
        else{
          ball[i] = 0;
        }
    }
    // atan2関数は -π ~ π までの範囲しかないから度数法に直した後180°=πradを足す
    ballAngle = degrees(atan2(ballY, ballX)) - 90;
    
    ballAngle = ballAngle < 0 ? ballAngle + 360 : ballAngle;
    ballAngle = ballAngle > 360 ? ballAngle - 360 : ballAngle;
    return;
}

void refreshBall16(void) {
    ballX = 0;
    ballY = 0;
    ballBig = 0;
    ballSum = 0;
    for (int i = 0; i < 16; i++) {
        ball[i] = 1023 - analogRead(ballPin[i]);
        ballSum += ball[i];
        // if(ball[i] >= 300){
        //     ballX += ball[i] * cos(radians(22.5*i + 90));
        //     ballY += ball[i] * sin(radians(22.5*i + 90));
        // }
        // else{
        //   ball[i] = 0;
        // }
        if(ball[ballBig] < ball[i]){
          ballBig = i;
        }
    }
    // ballAngle = degrees(atan2(ballY, ballX)) - 90;

    // ballAngle = ballAngle < -180 ? ballAngle + 360 : ballAngle;
    // ballAngle = ballAngle > 180 ? ballAngle - 360 : ballAngle;
    return;
}

void refreshBallPrint8(void) {
    refreshBall8();

    for (int i = 0; i < 8; i++) {
        Serial.print(ball[i*2]);
        Serial.print("\t");
    }
    Serial.println();
    return;
}

void refreshBallPrint16(void) {
    refreshBall16();

    // for (int i = 0; i < 16; i++) {
    //     Serial.print(ball[i]);
    //     Serial.print("\t");
    // }
    Serial.print(ballBig);
    Serial.print("\t");
    Serial.print(ballSum);
    Serial.print("\t");
    return;
}