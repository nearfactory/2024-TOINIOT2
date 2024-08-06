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


// -100~100[%]
int8_t motor[MOTOR_NUM] = { 0 };


inline void motorSetup(){
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
    pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 40000);
  }

  Serial.println("motor setup");
  return;
}

inline void motorRaw(int8_t m1, int8_t m2, int8_t m3, int8_t m4){

  m1 = m1<-100 ? -100 : m1;
  m1 = m1>100  ? 100  : m1;
  digitalWrite(MOTOR_PIN[0][MOTOR::PH], m1>0);
  analogWrite(MOTOR_PIN[0][MOTOR::EN], abs(m1));
  
  m2 = m2<-100 ? -100 : m2;
  m2 = m2>100  ? 100  : m2;
  digitalWrite(MOTOR_PIN[1][MOTOR::PH], m2>0);
  analogWrite(MOTOR_PIN[1][MOTOR::EN], abs(m2));

  m3 = m3<-100 ? -100 : m3;
  m3 = m3>100  ? 100  : m3;
  digitalWrite(MOTOR_PIN[2][MOTOR::PH], m3>0);
  analogWrite(MOTOR_PIN[2][MOTOR::EN], abs(m3));

  m4 = m4<-100 ? -100 : m4;
  m4 = m4>100  ? 100  : m4;
  digitalWrite(MOTOR_PIN[3][MOTOR::PH], m4>0);
  analogWrite(MOTOR_PIN[3][MOTOR::EN], abs(m4));

  return;
}

inline void motorP(){
  return;
}