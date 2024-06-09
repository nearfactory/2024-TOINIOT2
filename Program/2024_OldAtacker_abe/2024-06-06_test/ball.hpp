#pragma once 
#ifndef BALL_NOUSE

constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };

void ballSetup(){
  for(auto p:BALL_PIN) pinMode(p,INPUT);

  return;
}

void ballDebug(){
  //Serial.print("ball:");
  for(auto x:BALL_PIN){
    Serial.print(x);
    Serial.print(":");
    Serial.print(analogRead(x));
    Serial.print("\t");
  }
  Serial.println("\n");

  return;
}

void ballUpdate(short* ball){
  for(int i=0;i<BALL_NUM;i++){
    ball[i] = analogRead(BALL_PIN[i]);
  }

  return;
}

#endif