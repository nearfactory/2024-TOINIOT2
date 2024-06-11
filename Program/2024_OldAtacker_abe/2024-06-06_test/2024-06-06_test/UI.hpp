#pragma once
#ifndef UI_NOUSE

constexpr int BTN_NUM = 4;
const uint8_t BTN_PIN[BTN_NUM] = { 7, 8, 9, 10 };
constexpr int LED_NUM = 3;
const uint8_t LED_PIN[LED_NUM] = { 13, 14, 15 };

void buttonSetup(){
  for(auto p:BTN_PIN) pinMode(p, INPUT);
  return;
}

void LEDSetup(){
  for(auto p:LED_PIN) pinMode(p, OUTPUT);
  return;
}


void LEDWrite(bool* led){
  for(int i=0;i<LED_NUM;i++){
    digitalWrite(LED_PIN[i], led[i]);
  }
  return;
}

void buttonUpdate(bool *button){
  for(int i=0;i<BTN_NUM;i++){
    button[i] = digitalRead(BTN_PIN[i]);
  }
  return;
}

#endif