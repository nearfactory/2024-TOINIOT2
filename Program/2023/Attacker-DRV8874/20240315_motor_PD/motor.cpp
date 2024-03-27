// Arduino Include
#include <Arduino.h>

// Header Include
#include "motor.h"

// Other Include
#include "pin.h"
#include <math.h>

#define P_GAIN 1
#define D_GAIN 0.2

#define M_MIN 15


// PWM Frequency Config
/*
#include <avr/io.h>
void change_freq1(int divide){
  TCCR1B = (TCCR1B & 0b11111000) | divide;
}
*/

// Variables
float motorOutput[4];                     // モーター出力値
float motorOutputPrev[4] = {0, 0, 0, 0  // 前回モーター出力値
float motorDiffNow[4];                    // 今回偏差
float motorDiffPrev[4] = {0, 0, 0, 0};    // 前回偏差
float dt = 0.4;                         // ごく短い時間（微分に使用）

// 比例制御のステップ数
short int pStep = 10;

// バランス
short int m1b = 100;
short int m2b = 100;
short int m3b = 100;
short int m4b = 100;

// Function
void motorSetup(int p_step, short int m1_balance, short int m2_balance, short int m3_balance, short int m4_balance) {
  pStep = p_step;

  m1b = m1_balance > 100 ? 100 : m1_balance;
  m2b = m2_balance > 100 ? 100 : m2_balance;
  m3b = m3_balance > 100 ? 100 : m3_balance;
  m4b = m4_balance > 100 ? 100 : m4_balance;

  return;
}

void motorRaw(float m1, float m2, float m3, float m4, int mp) {

  // Checking Values with Ternary Operator
  m1 = m1 * mp * 0.01;
  m1 = m1 * m1b * 0.01;
  m1 = m1 > 100 ? 100 : m1;
  m1 = m1 < -100 ? -100 : m1;
  m1 = abs(m1) < M_MIN && m1 < -1 ? -1*M_MIN : m1;
  m1 = abs(m1) < M_MIN && m1 > 1 ? M_MIN : m1;
  m1 = abs(m1) <= 1 ? 0 : m1;

  m2 = m2 * mp * 0.01;
  m2 = m2 * m2b * 0.01;
  m2 = m2 > 100 ? 100 : m2;
  m2 = m2 < -100 ? -100 : m2;
  m2 = abs(m2) < M_MIN && m2 < -1 ? -1*M_MIN : m2;
  m2 = abs(m2) < M_MIN && m2 > 1 ? M_MIN : m2;
  m2 = abs(m2) <= 1 ? 0 : m2;

  m3 = m3 * mp * 0.01;
  m3 = m3 * m3b * 0.01;
  m3 = m3 > 100 ? 100 : m3;
  m3 = m3 < -100 ? -100 : m3;
  m3 = abs(m3) < M_MIN && m3 < -1 ? -1*M_MIN : m3;
  m3 = abs(m3) < M_MIN && m3 > 1 ? M_MIN : m3;
  m3 = abs(m3) <= 1 ? 0 : m3;

  m4 = m4 * mp * 0.01;
  m4 = m4 * m4b * 0.01;
  m4 = m4 > 100 ? 100 : m4;
  m4 = m4 < -100 ? -100 : m4;
  m4 = abs(m4) < M_MIN && m4 < -1 ? -1*M_MIN : m4;
  m4 = abs(m4) < M_MIN && m4 > 1 ? M_MIN : m4;
  m4 = abs(m4) <= 1 ? 0 : m4;

  // Serial.print(m1);
  // Serial.print("\t");
  // Serial.print(m2);
  // Serial.print("\t");
  // Serial.print(m3);
  // Serial.print("\t");
  // Serial.print(m4);
  // Serial.print("\n");

  // Output H/L
  if (m1 < 0) {
    digitalWrite(motorPin[0][0], 1);
  }
  else{
    digitalWrite(motorPin[0][0], 0);
  }

  if (m2 < 0) {
    digitalWrite(motorPin[1][0], 1);
  }
  else{
    digitalWrite(motorPin[1][0], 0);
  }

  if (m3 < 0) {
    digitalWrite(motorPin[2][0], 1);
  }
  else{
    digitalWrite(motorPin[2][0], 0);
  }

  if (m4 < 0) {
    digitalWrite(motorPin[3][0], 1);
  }
  else{
    digitalWrite(motorPin[3][0], 0);
  }

  m1 = m1 < 0 ? m1*(-1) : m1;
  m2 = m2 < 0 ? m2*(-1) : m2;
  m3 = m3 < 0 ? m3*(-1) : m3;
  m4 = m4 < 0 ? m4*(-1) : m4;

  // Output PWM
  analogWrite(motorPin[0][1], (int)m1*0.01*255);
  analogWrite(motorPin[1][1], (int)m2*0.01*255);
  analogWrite(motorPin[2][1], (int)m3*0.01*255);
  analogWrite(motorPin[3][1], (int)m4*0.01*255);

  // Serial.print((int)m1*0.01*255);
  // Serial.print("\t");
  // Serial.print((int)m2*0.01*255);
  // Serial.print("\t");
  // Serial.print((int)m3*0.01*255);
  // Serial.print("\t");
  // Serial.print((int)m4*0.01*255);
  // Serial.print("\n");

  return;
}

void motor(float m1, float m2, float m3, float m4, int mp) {
  motorRaw(m1, m2, m3, m4, mp);

  motorOutput[0] = m1;
  motorOutput[1] = m2;
  motorOutput[2] = m3;
  motorOutput[3] = m4;

  return;
}

void motorP(int m1G, int m2G, int m3G, int m4G, int mp) {
  for (int i = 0; i < pStep; i++) {
    motorDiffNow[0] = motorOutput[0] - motorOutputPrev[0];
    motorDiffNow[1] = motorOutput[1] - motorOutputPrev[1];
    motorDiffNow[2] = motorOutput[2] - motorOutputPrev[2];
    motorDiffNow[3] = motorOutput[3] - motorOutputPrev[3];

    motorOutputPrev[0] = motorOutput[0];
    motorOutputPrev[1] = motorOutput[1];
    motorOutputPrev[2] = motorOutput[2];
    motorOutputPrev[3] = motorOutput[3];

    // motorOutput[0] += ((m1G - motorOutput[0]) / (pStep - i) * P_GAIN);
    // motorOutput[1] += ((m2G - motorOutput[1]) / (pStep - i) * P_GAIN);
    // motorOutput[2] += ((m3G - motorOutput[2]) / (pStep - i) * P_GAIN);
    // motorOutput[3] += ((m4G - motorOutput[3]) / (pStep - i) * P_GAIN);

    motorOutput[0] += ((m1G - motorOutput[0]) / (pStep - i) * P_GAIN) + ((motorDiffNow[0] - motorDiffPrev[0]) / dt * D_GAIN);
    motorOutput[1] += ((m2G - motorOutput[1]) / (pStep - i) * P_GAIN) + ((motorDiffNow[1] - motorDiffPrev[1]) / dt * D_GAIN);
    motorOutput[2] += ((m3G - motorOutput[2]) / (pStep - i) * P_GAIN) + ((motorDiffNow[2] - motorDiffPrev[2]) / dt * D_GAIN);
    motorOutput[3] += ((m4G - motorOutput[3]) / (pStep - i) * P_GAIN) + ((motorDiffNow[3] - motorDiffPrev[3]) / dt * D_GAIN);

    Serial.print(-100);
    Serial.print("\t");
    Serial.print(motorOutput[0]);
    Serial.print("\t");
    Serial.print(motorOutput[1]);
    Serial.print("\t");
    Serial.print(motorOutput[2]);
    Serial.print("\t");
    Serial.print(motorOutput[3]);
    Serial.print("\t");
    Serial.print(100);
    Serial.print("\n");

    delay(10);

    motorRaw(motorOutput[0], motorOutput[1], motorOutput[2], motorOutput[3], mp);

    motorDiffPrev[0] = motorDiffNow[0];
    motorDiffPrev[1] = motorDiffNow[1];
    motorDiffPrev[2] = motorDiffNow[2];
    motorDiffPrev[3] = motorDiffNow[3];
  }
  return;
}