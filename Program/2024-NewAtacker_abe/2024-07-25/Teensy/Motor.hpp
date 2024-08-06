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

inline void motorRaw(){
  for(int i=0;i<MOTOR_NUM;i++){
    motor[i] = motor[i]<-100 ? -100 : motor[i];
    motor[i] = motor[i]>100  ? 100  : motor[i];
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], motor[i]>0);
    analogWrite(MOTOR_PIN[i][MOTOR::EN], abs(motor[i]*255/100));
  }

  return;
}

inline void motorRaw(int8_t m1, int8_t m2, int8_t m3, int8_t m4){
  motor[0] = m1;
  motor[1] = m2;
  motor[2] = m3;
  motor[3] = m4;

  motorRaw();
  return;
}

inline void motorP(){
  return;
}

inline void move_dir(double dir, short power){
  for(int i=0;i<MOTOR_NUM;i++){
    motor[i] = sin(dir-(45+i*90)*3.14/180.0)*power;
  }

  // motorRaw();
  return;
}

inline void setDir(){

  return;
}