#pragma once

constexpr uint8_t MOTOR_NUM = 4;
constexpr int MOTOR_PWM_TYPE = 2;
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

void motorSetup(){
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][0], OUTPUT);
    pinMode(MOTOR_PIN[i][1], OUTPUT);
  }

  return;
}