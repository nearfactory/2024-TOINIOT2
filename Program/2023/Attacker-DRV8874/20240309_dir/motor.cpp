// Arduino Include
#include <Arduino.h>

// Header Include
#include "motor.h"

// Other Include
#include "pin.h"
#include <math.h>

#define P_GAIN 1


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
short int M1 = 100;
short int M2 = 100;
short int M3 = 100;
short int M4 = 100;

// Function
void motorSetup(int p_step, short int m1_balance, short int m2_balance, short int m3_balance, short int m4_balance) {
  pStep = p_step;

  M1 = m1_balance;
  M2 = m2_balance;
  M3 = m3_balance;
  M4 = m4_balance;

  return;
}

void motorRaw(float m1, float m2, float m3, float m4, int mp) {

  // Checking Values with Ternary Operator
  m1 = m1 * mp * 0.01;
  m1 = m1 > 100 ? 100 : m1;
  m1 = m1 < -100 ? -100 : m1;
  m1 = abs(m1) < 10 && m1 < -1 ? -10 : m1;
  m1 = abs(m1) < 10 && m1 > 1 ? 10 : m1;
  m1 = abs(m1) <= 1 ? 0 : m1;

  m2 = m2 * mp * 0.01;
  m2 = m2 > 100 ? 100 : m2;
  m2 = m2 < -100 ? -100 : m2;
  m2 = abs(m2) < 10 && m2 < -1 ? -10 : m2;
  m2 = abs(m2) < 10 && m2 > 1 ? 10 : m2;
  m2 = abs(m2) <= 1 ? 0 : m2;

  m3 = m3 * mp * 0.01;
  m3 = m3 > 100 ? 100 : m3;
  m3 = m3 < -100 ? -100 : m3;
  m3 = abs(m3) < 10 && m3 < -1 ? -10 : m3;
  m3 = abs(m3) < 10 && m3 > 1 ? 10 : m3;
  m3 = abs(m3) <= 1 ? 0 : m3;

  m4 = m4 * mp * 0.01;
  m4 = m4 > 100 ? 100 : m4;
  m4 = m4 < -100 ? -100 : m4;
  m4 = abs(m4) < 10 && m4 < -1 ? -10 : m4;
  m4 = abs(m4) < 10 && m4 > 1 ? 10 : m4;
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

  Serial.print((int)m1*0.01*255);
  Serial.print("\t");
  Serial.print((int)m2*0.01*255);
  Serial.print("\t");
  Serial.print((int)m3*0.01*255);
  Serial.print("\t");
  Serial.print((int)m4*0.01*255);
  Serial.print("\n");

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
  m1G = m1G * mp;
  m2G = m2G * mp;
  m3G = m3G * mp;
  m4G = m4G * mp;

  for (int i = 0; i < pStep; i++) {
    motorNow[0] += (m1G - motorNow[0]) / pStep;
    motorNow[1] += (m2G - motorNow[1]) / pStep;
    motorNow[2] += (m3G - motorNow[2]) / pStep;
    motorNow[3] += (m4G - motorNow[3]) / pStep;
    motorRaw(motorNow[0], motorNow[1], motorNow[2], motorNow[3], 100);
  }
  return;
}