#pragma once

constexpr uint8_t MOTOR_NUM = 4;
constexpr uint8_t MOTOR_PWM_TYPE = 2;
const uint8_t MOTOR_PIN[MOTOR_NUM][MOTOR_PWM_TYPE]{
  {29, 28},
  {9, 6},
  {5, 4},
  {3, 2}
};
enum MOTOR : uint8_t{
  EN = 0,
  PH,
};


int16_t motor[MOTOR_NUM] = { 0 };


inline void motorSetup(){
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
    pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 40000);
  }

  Serial.println("motor setup");
  return;
}

inline void motorRaw(){
  for(int i=0;i<MOTOR_NUM;i++){
    analogWrite(MOTOR_PIN[i][MOTOR::EN], motor[i]);
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], motor[i]>0);  // ※
  }

  return;
}