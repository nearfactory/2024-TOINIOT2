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


// -100~100[%] の範囲で指定する
float motor         [MOTOR_NUM] = {0};  // プログラマ用

float motor_prev    [MOTOR_NUM] = {0};  // 前ループのプログラマ用の値
float motor_raw     [MOTOR_NUM] = {0};  // モーターに反映するやつ

float motor_p_step              = 8;    // P制御のステップ数
float motor_p_count [MOTOR_NUM] = {0};  // P制御のループ数カウント
float motor_p_val   [MOTOR_NUM] = {0};  // P制御の1ステップに変化する値

float move_dir                  = 0;

inline void motorSetup(){
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
    pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 40000);
  }
  
  Serial.println("motor setup");
  return;
}

inline void moveDir(double dir, short power, bool max_power){
  short power_big = 0;
  short power_big_id = 0;
  // Serial.print("default ");
  for(int i=0;i<MOTOR_NUM;i++){
    double t = (dir-45-i*90)*3.14/180.0;
    motor_raw[i] = sin(t)*power;
    // Serial.printf("m%d:%d ", i+1, motor[i]);
    if(max_power){
      if(abs(motor_raw[i]>power_big)){
        power_big = motor_raw[i];
        power_big_id = i;
      }
    }
  }
  double p = (double)power / abs(motor_raw[power_big_id]);
  if(!max_power) p = 1.0;
  // Serial.println();
  // Serial.printf("big:%d\n",power_big);
  // Serial.printf("id:%d\n",power_big_id);
  // Serial.printf("p:%lf\n",p);
  
  // Serial.print("max ");
  for(int i=0;i<MOTOR_NUM;i++){
    motor_raw[i] *= p;
    // Serial.printf("m%d:%d ", i+1, motor[i]);
  }
  // Serial.println();

  return;
}

inline void setDir(double dir, double goal_dir, uint8_t power, int blend){
  short mpDir = 0;
  dir -= goal_dir;
  dir = dir < -180 ? dir + 360 : dir;
  dir = dir > 180 ? dir - 360 : dir;

  blend = blend<0?0:blend;
  blend = blend>100?100:blend;
  double P_GAIN_DIR = 0.95;
  if(!(-10<dir && dir<10)){
    mpDir = (dir * P_GAIN_DIR);
    for(int i=0;i<4;i++){
      // motor[i] = motor[i]*(100-blend)*0.01 + mpDir*blend*0.01;
      motor_raw[i] += mpDir;
    }
  }

  return;
}

inline void motorP(){
  for(int i = 0; i < MOTOR_NUM; i++){
    if(motor_prev[i] == motor[i]  &&  motor_p_count[i] < motor_p_step){
      motor_raw[i] += motor_p_val[i];
      motor_p_count[i]++;
      // Serial.print("moving");
    }else if(motor_prev[i] != motor[i]){
      motor_p_val[i] = (motor[i] - motor_raw[i]) / motor_p_step;

      motor_raw[i] += motor_p_val[i];
      motor_p_count[i] = 1;
      // Serial.print("differ");
    }else{
      // Serial.print("none");
    }

    motor_prev[i] = motor[i];
  }
  return;
}

inline void motorRaw(){
  for(int i=0;i<MOTOR_NUM;i++){
      motor_raw[i] = motor_raw[i]<-100.0 ? -100.0 : motor_raw[i];
      motor_raw[i] = 100.0<motor_raw[i]  ?  100.0 : motor_raw[i];

      digitalWrite(MOTOR_PIN[i][MOTOR::PH], motor_raw[i]>0);
      analogWrite(MOTOR_PIN[i][MOTOR::EN], (uint8_t)abs(motor_raw[i]*255.0/100.0));
  }

  return;
}