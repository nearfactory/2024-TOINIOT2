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

float motor_p_step_default      = 4;                    // デフォルトのP制御のステップ数
float motor_p_step              = motor_p_step_default; // P制御のステップ数
float motor_p_count [MOTOR_NUM] = {0};                  // P制御のループ数カウント
float motor_p_val   [MOTOR_NUM] = {0};                  // P制御の1ステップに変化する値

float move_dir                  = 0;

inline void motorSetup(){
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
    pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 100000);
  }
  
  Serial.println("motor setup");
  return;
}

inline void motorSet(float m1, float m2, float m3, float m4){
  motor[0] = m1;
  motor[1] = m2;
  motor[2] = m3;
  motor[3] = m4;

  return;
}

inline void moveDir(double dir, int power, bool max_power, int blend){
  int power_big = 0;
  int power_big_id = 0;
  
  for(int i=0;i<MOTOR_NUM;i++){
    double t = (dir-45-i*90)*3.14/180.0;
    motor[i] = sin(t)*power;
    if(max_power){
      if(abs(motor[i]>power_big)){
        power_big = motor[i];
        power_big_id = i;
      }
    }
  }
  double p = (double)power / abs(motor[power_big_id]);
  if(!max_power) p = 1.0;
  // Serial.printf("big:%d\n",power_big);
  // Serial.printf("id:%d\n",power_big_id);
  // Serial.printf("p:%lf\n",p);
  
  // Serial.print("max ");
  for(int i=0;i<MOTOR_NUM;i++){
    motor[i] *= p;
    // Serial.printf("m%d:%d ", i+1, motor[i]);
  }
  // Serial.println();

  return;
}

inline void setDir(double dir, double goal_dir, double power, int blend){
  double dir_power = dir/1.8;
  for(int i=0;i<MOTOR_NUM;i++){
    motor[i] = (dir_power * power/100.0)*blend/100.0 + motor[i]*(100.0-blend)/100.0;
  }
  // Serial.printf("motor:%lf motor_raw:%lf, dir:%lf, def_dir:%lf\n", motor[0], motor_raw[0], dir, default_dir);

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