#pragma once

using namespace std;

namespace{
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 7,8,9,10 };

  constexpr uint8_t LED_NUM = 3;
  const     uint8_t LED_PIN[LED_NUM] = { 13,14,15 };

  constexpr uint8_t BZ_PIN = 6;
}


bool  button[BUTTON_NUM] = {false};
bool  previous_button[BUTTON_NUM] = {false};
bool  LED[LED_NUM] = {false};
float bz = -1.0f;


// セットアップ関数（.inoのsetup()内で呼び出し）
inline void UISetup(){
  // pinMode変更
  for(auto p:BUTTON_PIN)  pinMode(p, INPUT);
  for(auto p:LED_PIN)     pinMode(p, OUTPUT);
  pinMode(BZ_PIN, OUTPUT);
  
  Serial.println("ui setup");
  return;
}

// ボタンのH/Lを取得してグローバル変数を更新
inline void buttonUpdate(){
  for(int i=0;i<BUTTON_NUM;i++){
    previous_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

// ボタンのH/Lの切り替わりをT/Fで返す
inline bool buttonUp(uint8_t num){
  num = num<1 ? 1 : num;
  num = num>5 ? 5 : num;
  return (!button[num-1]) && previous_button[num-1];
}

inline void LEDupdate(){
  for(int i=0;i<LED_NUM;i++) digitalWrite(LED_PIN[i], LED[i]);
  return;
}

// グローバル変数に格納されている周波数でブザーに出力
inline void bzUpdate(){
  if(bz > -1.0f){
    tone(BZ_PIN, bz);
  }
  return;
}