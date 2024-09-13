#pragma once

#include <string>
#include <vector>

using namespace std;

namespace{
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 36,37,31,30 };

  constexpr uint8_t BZ_PIN = 33;
}


bool                button[BUTTON_NUM] = {false};
bool                previous_button[BUTTON_NUM] = {false};
float               bz = -1.0f;


// セットアップ関数（.inoのsetup()内で呼び出し）
inline void UISetup(){
  // pinMode変更
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);
  pinMode(BZ_PIN, OUTPUT);

  // ディスプレイ初期化
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Display error!");
    while(true);
  }
  display.setTextColor(WHITE);
  display.setTextSize(1);
  
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

// グローバル変数に格納されている周波数でブザーに出力
inline void bzUpdate(){
  if(bz > -1.0f){
    tone(BZ_PIN, bz);
  }
  return;
}