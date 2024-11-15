#include "Motor.hpp"

void Motor::begin(){
  for(int i=0;i<NUM;i++){
    pinMode(PIN[i][EN], OUTPUT);
    pinMode(PIN[i][PH], OUTPUT);
    analogWriteFrequency(PIN[i][EN], FREQUENCY);
  }
  
  Serial.println("motor setup");
  return;
}

void Motor::set(float m1, float m2, float m3, float m4){
  motor[0] = m1;
  motor[1] = m2;
  motor[2] = m3;
  motor[3] = m4;

  return;
}

void Motor::moveDir(float dir, uint8_t power){
  for(int i=0;i<NUM;i++){
    float t = (-dir-45-i*90)*3.14/180.0;
    motor[i] = sin(t)*power;
  }

  return;
}

void Motor::p(){
  for(int i = 0; i < NUM; i++){
    if(motor_prev[i] == motor[i]  &&  p_count[i] < p_step){
      motor_raw[i] += p_val[i];
      p_count[i]++;
    }else if(motor_prev[i] != motor[i]){
      p_val[i] = (motor[i] - motor_raw[i]) / p_step;

      motor_raw[i] += p_val[i];
      p_count[i] = 1;
    }

    motor_prev[i] = motor[i];
  }
  return;
}

void Motor::avr(){
  static int queue_index = 0;      // 出力値のキューのインデックス

  for(int i=0;i<NUM;i++){
    queue[queue_index][i] = motor[i];
    queue_index = (queue_index+1) % QUEUE_SIZE;

    // 出力値を計算
    float sum = 0.0f;
    for(int j=0;j<QUEUE_SIZE;j++) sum += queue[j][i];
    motor_raw[i] = sum / QUEUE_SIZE;
  }

}

void Motor::write(){
  for(int i=0;i<NUM;i++){
      motor_raw[i] = motor_raw[i]<-100.0 ? -100.0 : motor_raw[i];
      motor_raw[i] = 100.0<motor_raw[i]  ?  100.0 : motor_raw[i];

      digitalWrite( PIN[i][PH], motor_raw[i]>0);
      analogWrite(  PIN[i][EN], (uint8_t)abs(motor_raw[i]*255/100));
  }

  return;
}