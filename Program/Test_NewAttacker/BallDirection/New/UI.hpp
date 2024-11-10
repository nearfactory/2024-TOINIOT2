#include "core_pins.h"
#pragma once

#include <string>
#include <vector>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>


using namespace std;

constexpr uint8_t BUTTON_NUM = 4;
const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 37,31,30,32 };
          bool    button[BUTTON_NUM] = {false};
          bool    previous_button[BUTTON_NUM] = {false};

constexpr uint8_t TOGGLE_PIN = 36;

constexpr uint8_t BZ_PIN = 33;
float               bz = -1.0f;

constexpr uint8_t DISPLAY_W = 128;
constexpr uint8_t DISPLAY_H = 64;
constexpr int8_t  DISPLAY_RESET = -1;
constexpr uint8_t DISPLAY_ADDR = 0x3c;
constexpr uint8_t DISPLAY_MODE_NUM = 9;
const std::string DISPLAY_MODE_NAME[DISPLAY_MODE_NUM] = {
  "ball",
  "ble",
  "camera",
  "dir",
  "dribbler",
  "kicker",
  "line",
  "motor",
  "variables"
};

enum DISPLAY_MODE : uint8_t{
  BALL = 0,
  BLE,
  CAMERA,
  DIR,
  DRIBBLER,
  KICKER,
  LINE,
  MOTOR,
  VARIABLES
};
enum class TEXT_ALIGN : uint8_t{
  LEFT = 0,
  CENTER,
  RIGHT,
  
  TOP,
  MIDDLE,
  BOTTOM
};


Adafruit_SSD1306    display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET);
bool                use_display = false;
uint8_t             display_mode = 0;
vector<std::string> debug_variables(0);



// セットアップ関数（.inoのsetup()内で呼び出し）
inline void UISetup(){
  // pinMode変更
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);
  pinMode(TOGGLE_PIN, INPUT_PULLUP);
  pinMode(BZ_PIN, OUTPUT);

  // ディスプレイ初期化
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Display err!");
    while(true);
  }
  display.setTextColor(WHITE);
  display.setTextSize(1);
  
  Serial.println("ui setup");
  return;
}

// ボタンのH/Lを取得してグローバル変数を更新
inline void buttonRead(){
  for(int i=0;i<BUTTON_NUM;i++){
    previous_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

// ボタンのH/Lの切り替わりをT/Fで返す
inline bool buttonUp(uint8_t num){
  num = num<0 ? 0 : num;
  num = num>3 ? 3 : num;
  return (!button[num]) && previous_button[num];
}

// グローバル変数に格納されている周波数でブザーに出力
inline void bzWrite(){
  if(bz > -1.0f){
    tone(BZ_PIN, bz);
  }
  return;
}

// display
inline void printd(uint8_t x, uint8_t y, std::string str, TEXT_ALIGN align_x = TEXT_ALIGN::LEFT, TEXT_ALIGN align_y = TEXT_ALIGN::TOP){
  switch(align_x){
    case TEXT_ALIGN::LEFT :
      break;
    case TEXT_ALIGN::CENTER :
      x -= str.length()*8/2;
      break;
    case TEXT_ALIGN::RIGHT :
      x -= str.length()*8;
      break;
  }
  switch(align_y){
    case TEXT_ALIGN::TOP :
      break;
    case TEXT_ALIGN::MIDDLE :
      y -= 8/2;
      break;
    case TEXT_ALIGN::BOTTOM :
      y -=8;
      break;
  }
  display.setCursor(x, y);
  display.print(str.c_str());

  return;
}

inline void drawAngleLine(uint8_t center_x, uint8_t center_y, float angle, uint8_t radius){
  display.drawLine(center_x, center_y, center_x+cos(radians(angle))*radius, center_y+sin(radians(angle))*radius, WHITE);
  return;
}