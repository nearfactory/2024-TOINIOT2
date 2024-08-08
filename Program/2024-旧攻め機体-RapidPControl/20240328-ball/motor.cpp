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
extern float mpGoal[4] = {0, 0, 0, 0};
extern float mpGoalPrev[4] = {0, 0, 0, 0};
extern float mpOutput[4] = {0, 0, 0, 0};
short int m1b = 100;
short int m2b = 100;
short int m3b = 100;
short int m4b = 100;

short int pStep = 10;

// Function
void motorSetup(short int p_step, short int m1_balance, short int m2_balance, short int m3_balance, short int m4_balance) {
  m1b = m1_balance > 100 ? 100 : m1_balance;
  m2b = m2_balance > 100 ? 100 : m2_balance;
  m3b = m3_balance > 100 ? 100 : m3_balance;
  m4b = m4_balance > 100 ? 100 : m4_balance;

  pStep = p_step;

  return;
}

void move(void) {
  float m1 = mpOutput[0];
  float m2 = mpOutput[1];
  float m3 = mpOutput[2];
  float m4 = mpOutput[3];

  // Checking Values with Ternary Operator
  m1 = m1 * m1b * 0.01;
  m1 = m1 > 100 ? 100 : m1;
  m1 = m1 < -100 ? -100 : m1;
  m1 = abs(m1) < M_MIN && m1 < -1 ? -1*M_MIN : m1;
  m1 = abs(m1) < M_MIN && m1 > 1 ? M_MIN : m1;
  m1 = abs(m1) <= 1 ? 0 : m1;

  m2 = m2 * m2b * 0.01;
  m2 = m2 > 100 ? 100 : m2;
  m2 = m2 < -100 ? -100 : m2;
  m2 = abs(m2) < M_MIN && m2 < -1 ? -1*M_MIN : m2;
  m2 = abs(m2) < M_MIN && m2 > 1 ? M_MIN : m2;
  m2 = abs(m2) <= 1 ? 0 : m2;

  m3 = m3 * m3b * 0.01;
  m3 = m3 > 100 ? 100 : m3;
  m3 = m3 < -100 ? -100 : m3;
  m3 = abs(m3) < M_MIN && m3 < -1 ? -1*M_MIN : m3;
  m3 = abs(m3) < M_MIN && m3 > 1 ? M_MIN : m3;
  m3 = abs(m3) <= 1 ? 0 : m3;

  m4 = m4 * m4b * 0.01;
  m4 = m4 > 100 ? 100 : m4;
  m4 = m4 < -100 ? -100 : m4;
  m4 = abs(m4) < M_MIN && m4 < -1 ? -1*M_MIN : m4;
  m4 = abs(m4) < M_MIN && m4 > 1 ? M_MIN : m4;
  m4 = abs(m4) <= 1 ? 0 : m4;

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

  return;
}

void mpCalc(float mp1, float mp2, float mp3, float mp4){
  mpGoal[0] = mp1;
  mpGoal[1] = mp2;
  mpGoal[2] = mp3;
  mpGoal[3] = mp4;

  float P_mpOperation;

  for(int i=0; i<4; i++){
    if(mpGoal[i] != mpGoalPrev[i]){
      P_mpOperation = (mpGoal[i] - mpOutput[i]) / pStep;
    }
    mpOutput[i] += P_mpOperation;
  }
}