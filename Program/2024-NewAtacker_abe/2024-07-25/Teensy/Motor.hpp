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
int8_t motor[MOTOR_NUM] = {0};
int8_t motor_raw[MOTOR_NUM] = {0};
int8_t motor_now[MOTOR_NUM] = {0};
short motor_sum = 0;
double move_dir = 0;

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
  // motor_sum = motor[0]+motor[1]+motor[2]+motor[3];
  for(int i=0;i<MOTOR_NUM;i++){
    // motor[i] = motor[i]<-100 ? -100 : motor[i];
    // motor[i] = motor[i]>100  ? 100  : motor[i];
      digitalWrite(MOTOR_PIN[i][MOTOR::PH], motor_raw[i]>0);
      analogWrite(MOTOR_PIN[i][MOTOR::EN], abs(motor_raw[i]*255.0/100.0));
    // if(!(motor_sum>80&&accel_sum<10.4)){
    // }else{
    //   analogWrite(MOTOR_PIN[i][MOTOR::EN], 0);
    // }
  }

  return;
}

inline void motorRaw(int8_t m1, int8_t m2, int8_t m3, int8_t m4){
  motor_raw[0] = m1;
  motor_raw[1] = m2;
  motor_raw[2] = m3;
  motor_raw[3] = m4;

  motorRaw();
  return;
}

int p_step = 10;
int p_count[MOTOR_NUM] = 0;
int p_val[MOTOR_NUM] = 0;

int motor_goal[MOTOR_NUM] = {};
int motor_goal_prev[MOTOR_NUM] = {};


inline void motorP(){
  for(int i = 0; i < MOTOR_NUM; i++){

    motor_goal[i] = motor_raw[i];
    
    motor_goal[i] = motor_goal[i] > -255 ? motor_goal[i] : -255;
    motor_goal[i] = motor_goal[i] < 255  ? motor_goal[i] :  255;

    if(motor_goal_prev[i] == motor_goal[i]  &&  p_count[i] < p_step){
      motor_now[i] += p_val[i];
      p_count[i]++;
    }else if(motor_goal_prev[i] != motor_goal[i]){
      p_val[i] = (motor_goal[i] - motor_now[i]) / p_step;
      motor_now[i] += p_val[i];
      p_count[i] = 1;
    }

    motor_raw[i] = motor_goal[i];
    motor_goal_prev[i] = motor_goal[i];
  }
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

constexpr auto P_STEP = 4;

/*
inline void motorRapidP(short m1,short m2,short m3,short m4){
  for(int i=0;i<4;i++) prev_mp_goal[i] = mp_goal[i];

  short p_mp_operation = 0.0f;

  for(int i=0;i<MOTOR_NUM;i++){
    if(motor[i]!=prev_mp_goal[i]){
      p_mp_operation = (motor[i]-motor_raw[i]) / p_step;
    }
    motor_raw[i] += p_mp_operation;
  }

  return;
}
*/
