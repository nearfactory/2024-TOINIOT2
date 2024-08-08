// Arduino Include
#include <Arduino.h>

// Header Include
#include "motor.h"

// Other Include
#include "pin.h"
#include <math.h>

#define P_GAIN 1
#define M_MIN 15


// PWM Frequency Config
/*
#include <avr/io.h>
void change_freq1(int divide){
  TCCR1B = (TCCR1B & 0b11111000) | divide;
}
*/

// Variables
float motorNow[4];
short int pStep = 10;
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
    digitalWrite(motorPin[0][0], 0);
  }
  else{
    digitalWrite(motorPin[0][0], 1);
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
    digitalWrite(motorPin[3][0], 0);
  }
  else{
    digitalWrite(motorPin[3][0], 1);
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

  motorNow[0] = m1;
  motorNow[1] = m2;
  motorNow[2] = m3;
  motorNow[3] = m4;

  return;
}

void motorP(int m1G, int m2G, int m3G, int m4G, int mp) {
  m1G = m1G;
  m2G = m2G;
  m3G = m3G;
  m4G = m4G;

  for (int i = 0; i < pStep; i++) {
    motorNow[0] += (m1G - motorNow[0]) / (pStep - i);
    motorNow[1] += (m2G - motorNow[1]) / (pStep - i);
    motorNow[2] += (m3G - motorNow[2]) / (pStep - i);
    motorNow[3] += (m4G - motorNow[3]) / (pStep - i);

    // Serial.print(-100);
    // Serial.print("\t");
    // Serial.print(motorNow[0]);
    // Serial.print("\t");
    // Serial.print(motorNow[1]);
    // Serial.print("\t");
    // Serial.print(motorNow[2]);
    // Serial.print("\t");
    // Serial.print(motorNow[3]);
    // Serial.print("\t");
    // Serial.print(100);
    // Serial.print("\n");

    motorRaw(motorNow[0], motorNow[1], motorNow[2], motorNow[3], mp);
  }
  return;
}