// #pragma once

// constexpr uint8_t MOTOR_NUM = 4;
// constexpr uint8_t MOTOR_PWM_TYPE = 2;
// const uint8_t MOTOR_PIN[MOTOR_NUM][MOTOR_PWM_TYPE]{
//   {29, 28},
//   {9, 6},
//   {5, 4},
//   {3, 2}
// };
// enum MOTOR : uint8_t{
//   EN = 0,
//   PH,
// };


// // -100~100[%] の範囲で指定する
// float motor         [MOTOR_NUM] = {0};  // プログラマ用

// float motor_prev    [MOTOR_NUM] = {0};  // 前ループのプログラマ用の値
// float motor_raw     [MOTOR_NUM] = {0};  // モーターに反映するやつ

// float move_dir                  = 0;

// constexpr int SAMPLE_NUM = 5;   // 移動平均のサンプル数
// int motor_queue[SAMPLE_NUM]{};  // 出力値のキュー
// int motor_queue_index = 0;      // 出力値のキューのインデックス

// inline void motorSetup(){
//   for(int i=0;i<MOTOR_NUM;i++){
//     pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
//     pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
//     analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 100000);
//   }
  
//   Serial.println("motor setup");
//   return;
// }

// inline void motorSet(float m1, float m2, float m3, float m4){
//   motor[0] = m1;
//   motor[1] = m2;
//   motor[2] = m3;
//   motor[3] = m4;

//   return;
// }

// inline void moveDir(double dir, int power){
//   for(int i=0;i<MOTOR_NUM;i++){
//     float t = (-dir-45-i*90)*3.14/180.0;
//     motor[i] = sin(t)*power;
//   }

//   return;
// }

// // inline void setDir(double dir, double goal_dir, double power, int blend){
// //   double dir_power = dir/1.8;
// //   for(int i=0;i<MOTOR_NUM;i++){
// //     motor[i] = (dir_power * power/100.0)*blend/100.0 + motor[i]*(100.0-blend)/100.0;
// //   }
// //   // Serial.printf("motor:%lf motor_raw:%lf, dir:%lf, def_dir:%lf\n", motor[0], motor_raw[0], dir, default_dir);

// //   return;
// // }

// inline void motorAvr(){
//   for(int i=0;)
//   motor_queue[motor_queue_index] = motor[i];
//   motor_queue_index = (motor_queue_index+1) % SAMPLE_NUM;

//   // 出力値を計算
//   for(auto m:motor_queue) motor_sum += m;
//   motor_raw = motor_sum / SAMPLE_NUM;

// }

// inline void motorRaw(){
//   for(int i=0;i<MOTOR_NUM;i++){
//       motor_raw[i] = motor_raw[i]<-100.0 ? -100.0 : motor_raw[i];
//       motor_raw[i] = 100.0<motor_raw[i]  ?  100.0 : motor_raw[i];

//       digitalWrite(MOTOR_PIN[i][MOTOR::PH], motor_raw[i]>0);
//       analogWrite(MOTOR_PIN[i][MOTOR::EN], (uint8_t)abs(motor_raw[i]*255.0/100.0));
//   }

//   return;
// }