#pragma once
#ifndef MOTOR_NOUSE

#include "Arduino.h"
#include <math.h>

constexpr int MOTOR_NUM = 4;
const uint8_t MOTOR_PIN[MOTOR_NUM][2] = { { 19, 3 }, { 17, 2 }, { 25, 5 }, { 23, 4 } };
enum MOTOR : uint8_t{
  LF=0,
  LB,
  RB,
  RF
};

void motorSetup(){
  for(int j=0;j<MOTOR_NUM;j++){
    for(int i=0;i<2;i++)
      pinMode(MOTOR_PIN[j][i], OUTPUT);
  }

  return;
}

void motorDebug(){
  static int count=0;
  int power = sin(count/40.0)*255;
  for(int i=0;i<4;i++){
    digitalWrite(MOTOR_PIN[i][0], power<0?0:1 );
    analogWrite(MOTOR_PIN[i][1], abs(power));
  }
  
  count++;
  return;
}

// 15以下は動かない？
// blend 0-100
void setDir(uint8_t* motor, double dir, double default_dir, uint8_t power, int blend){
  for(int i=0;i<MOTOR_NUM;i++){
    double diffrence = dir - default_dir;
    diffrence = fmod(fmod(diffrence,360.0)+360.0,360.0);
    //motor[i] = power;
  }  
  return;
}

void setMove(uint8_t* motor, double dir, uint8_t power, int blend){

  return;
}

void setMotor(uint8_t* motor){
  for(int i=0;i<MOTOR_NUM;i++){
    analogWrite(MOTOR_PIN[i], motor[i]);
  }
  return;
}

#endif