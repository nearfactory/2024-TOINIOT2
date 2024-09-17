#pragma once
#ifndef MOTOR_NOUSE

#include "Arduino.h"
#include <math.h>

constexpr int MOTOR_NUM = 4;
const uint8_t MOTOR_PIN[MOTOR_NUM][2] = { { 19, 3 }, { 17, 2 }, { 25, 5 }, { 23, 4 } };
const uint8_t DEFAULT_MOTOR_DIR[MOTOR_NUM] = {1,0,0,1};
enum MOTOR : uint8_t{
  LF = 0,
  LB,
  RB,
  RF,

  PH = 0,
  EN,
};
enum class DIR : uint8_t{
  FRONT = 0,
  FRONT_LEFT,
  LEFT,
  BACK_LEFT,
  BACK,
  BACK_RIGHT,
  RIGHT,
  FRONT_RIGHT,
};

inline void motorSetup();
inline void motorDebug();
inline void motorRaw(short* motor, short v1, short v2, short v3, short v4);
inline void motorRaw(short* motor, short v1, short v2, short v3, short v4, short power);
inline void motorRaw(short* motor, DIR dir, short power);
inline void motorP(short* motor, short v1, short v2, short v3, short v4);
inline void setDir(short* motor, double dir, double goal_dir, uint8_t power, int blend);
inline void setMove(uint8_t* motor, double dir, uint8_t power, int blend);
inline void setMotor(short* motor);


inline void motorSetup(){
  for(int j=0;j<MOTOR_NUM;j++){
    for(int i=0;i<2;i++)
      pinMode(MOTOR_PIN[j][i], OUTPUT);
  }

  return;
}

inline void motorDebug(){
  static int count=0;
  int power = sin(count/40.0)*255;
  for(int i=0;i<4;i++){
    digitalWrite(MOTOR_PIN[i][0], power<0?0:1);
    analogWrite(MOTOR_PIN[i][1], abs(power));
  }
  
  count++;
  return;
}


inline void motorRaw(short* motor, short v1, short v2, short v3, short v4){
  motor[0] = v1;
  motor[1] = v2;
  motor[2] = v3;
  motor[3] = v4;
  
  return;
}

inline void motorRaw(short* motor, short v1, short v2, short v3, short v4, short power){
  motor[0] = v1*power;
  motor[1] = v2*power;
  motor[2] = v3*power;
  motor[3] = v4*power;

  return;
}

// ちゃんと動く？
inline void motorRaw(short* motor, DIR dir, short power){
  switch(dir){
    case DIR::FRONT:
      motorRaw(motor, power, power, -power, -power);
      break;
    case DIR::FRONT_LEFT:
      motorRaw(motor, 0, power, 0, -power);
      break;
    case DIR::LEFT:
      motorRaw(motor, -power, power, power, -power);
      break;
    case DIR::BACK_LEFT:
      motorRaw(motor, -power, 0, power, 0);
      break;
    case DIR::BACK:
      motorRaw(motor, -power, -power, power, power);
      break;
    case DIR::BACK_RIGHT:
      motorRaw(motor, 0, -power, 0, power);
      break;
    case DIR::RIGHT:
      motorRaw(motor, power, -power, -power, power);
      break;
    case DIR::FRONT_RIGHT:
      motorRaw(motor, power, 0, -power, 0);
      break;
  }

  return;
}


inline void motorP(short* motor, short v1, short v2, short v3, short v4){
  static short motor_now[MOTOR_NUM]={0.0f};
  // for(auto&m:motor) m += ()
  int p_step=64;
  for(int i=0;i<p_step;i++){
    motor[0] += (v1-motor[0]) / (p_step-i);
    motor[1] += (v1-motor[1]) / (p_step-i);
    motor[2] += (v1-motor[2]) / (p_step-i);
    motor[3] += (v1-motor[3]) / (p_step-i);
    setMotor(motor);
  }

}

// blend 0-100
inline void setDir(short* motor, double dir, double goal_dir, uint8_t power, int blend){
  short mpDir = 0;
  dir -= goal_dir;
  dir = dir < -180 ? dir + 360 : dir;
  dir = dir > 180 ? dir - 360 : dir;

  blend = blend<0?0:blend;
  blend = blend>100?100:blend;
  double P_GAIN_DIR = 0.95;
  if(!(-10<dir && dir<10)){
    mpDir = (dir * P_GAIN_DIR) * (-1) * power / 100;
    for(int i=0;i<4;i++){
      motor[i] = motor[i]*(100-blend)*0.01 + mpDir*blend*0.01;
    }
    // Serial.print("dir:");
    // Serial.println(dir);
    // Serial.print("mpDir:");
    // Serial.println(mpDir);
  }
  
  return;
}

inline void setMove(uint8_t* motor, double dir, uint8_t power, int blend){

  return;
}

inline void setMotor(short* motor){
  for(int i=0;i<MOTOR_NUM;i++){
    bool flag = DEFAULT_MOTOR_DIR[i];
    if(motor[i]<0) flag = !flag;
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], flag);
    analogWrite(MOTOR_PIN[i][MOTOR::EN], abs(motor[i]*255*0.01));
  }
  return;
}

#endif