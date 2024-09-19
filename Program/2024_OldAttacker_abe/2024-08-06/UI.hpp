#pragma once

#include <Wire.h>

using namespace std;

namespace{
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 7,8,9,10 };

  constexpr uint8_t LED_NUM = 3;
  const     uint8_t LED_PIN[LED_NUM] = {13,14,15};

  constexpr uint8_t BUZZER_PIN = 6;
}

bool                button[BUTTON_NUM] = {false};
bool                previous_button[BUTTON_NUM] = {false};
float               buzzer = -1.0f;

inline void UISetup(){
  // button
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);

  // buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  return;
}

// button
inline void buttonUpdate(){
  for(int i=0;i<BUTTON_NUM;i++){
    previous_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

inline bool buttonUp(uint8_t num){
  num--;
  num = num<0 ? 0 : num;
  num = num>BUTTON_NUM-1 ? BUTTON_NUM-1 : num;
  return (!button[num]) && previous_button[num];
}

inline void buzzerUpdate(void){
  tone(BUZZER_PIN, 440.0f);
  return;
}

inline void buzzerUpdate(float frequency){
  tone(BUZZER_PIN, frequency);
  return;
}

// buzzer
/*
inline void buzzerUpdate(){
  if(buzzer > -1.0f){
    tone(BUZZER_PIN, buzzer);
  }
  return;
}
*/