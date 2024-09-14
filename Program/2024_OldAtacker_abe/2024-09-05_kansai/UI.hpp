#pragma once

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Ball.hpp"
#include "Dir.hpp"

using namespace std;

namespace{
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 7,8,9,10 };

  constexpr uint8_t LED_NUM = 3;
  const     uint8_t LED_PIN[LED_NUM] = { 13,14,15 };

  constexpr uint8_t BZ_PIN = 6;

  constexpr uint8_t DISPLAY_W = 128;
  constexpr uint8_t DISPLAY_H = 64;
  constexpr uint8_t DISPLAY_RESET = -1;
  constexpr uint8_t DISPLAY_ADDR = 0x3c;
  constexpr uint8_t DISPLAY_MODE_NUM = 9;
  const std::string DISPLAY_MODE_NAME[DISPLAY_MODE_NUM] = {
    "ball",
    "camera",
    "dir",
    "line",
    "motor",
    "variables"
  };

}
enum DISPLAY_MODE : uint8_t{
  BALL = 0,
  CAMERA,
  DIR,
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


bool                button[BUTTON_NUM] = {false};
bool                previous_button[BUTTON_NUM] = {false};
bool                led[LED_NUM] = {false};
float               bz = -1.0f;
Adafruit_SSD1306    display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET);
bool                use_display = false;
uint8_t             DISPLAY_MODE = 0;
vector<std::string> debug_variables(0);



// セットアップ関数（.inoのsetup()内で呼び出し）
inline void UISetup(){
  // pinMode変更
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);
  for(auto p:LED_PIN)    pinMode(p, OUTPUT);
  pinMode(BZ_PIN, OUTPUT);

  // ディスプレイ初期化
  /*
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Display error!");
    while(true);
  }
  */
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
  num = num<0 ? 0 : num;
  num = num>3 ? 3 : num;
  return (!button[num]) && previous_button[num];
}

inline void LEDUpdate(){
  for(int i=0;i<LED_NUM;i++){
    digitalWrite(LED_PIN[i], led[i]);
  }
  return;
}

// グローバル変数に格納されている周波数でブザーに出力
inline void bzUpdate(){
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

inline void drawAngleLine(uint8_t center_x, uint8_t center_y, float angle, uint8_t r){
  display.drawLine(center_x, center_y, center_x+cos(angle*3.14/180)*r, center_y+sin(angle*3.14/180)*r, WHITE);
  return;
}

inline void clearVariables(){
  debug_variables.clear();
  debug_variables_addr.clear();
}

template <typename T>
inline void addVariables(std::string name, T variables){
  debug_variables.emplace_back(name+":"+to_string(variables));
  return;
}

inline void debugDisplay(uint8_t mode){
  printd(8,8,DISPLAY_MODE_NAME[mode]);

  switch(mode){
    case DISPLAY_MODE::BALL :{
      uint8_t circle_r = 24;
      uint8_t text_r = circle_r + 4;

      string str = to_string(ball_dir);
      str.erase(str.begin()+4,str.end());
      printd(8, 40, str);

      printd(8,48,"exist:"+to_string(ball_exist));
      printd(8,56,"hold :"+to_string(ball_holding));
      
      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, -ball_dir-180, circle_r);
      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 0, 8);

      printd(120,40,to_string(ball_small),    TEXT_ALIGN::RIGHT);
      printd(120,48,to_string(ball_big),      TEXT_ALIGN::RIGHT);
      printd(120,56,to_string(ball_distance), TEXT_ALIGN::RIGHT);
      for(int i=0;i<BALL_NUM;i++){
        double angle = (i*360/BALL_NUM+180);
        angle = angle/180*3.14;
        uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
        uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
        display.drawPixel(x, y, WHITE);
      }
      break;
    }

    case DISPLAY_MODE::CAMERA :{
      printd(64,32,"no data", TEXT_ALIGN::CENTER, TEXT_ALIGN::MIDDLE);
      break;
    }

    case DISPLAY_MODE::DIR :{
      uint8_t circle_r = 24;
      string str = to_string(dir);
      str.erase(str.begin()+5,str.end());
      printd(8, 32, str, TEXT_ALIGN::LEFT, TEXT_ALIGN::MIDDLE);

      // display.drawPixel(DISPLAY_W/2+24*cos((180-prev_dir)*3.14/180.0),DISPLAY_H/2+24*sin((180-prev_dir)*3.14/180.0),WHITE);
      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 180, 24);
      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 180-dir, 16);

      display.drawPixel(DISPLAY_W/2+accel_x*6, DISPLAY_H/2+accel_y*6, WHITE);

      for(int i=0;i<8;i++){
        double angle = i*360/8 -dir;
        angle = angle/180*3.14;
        uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
        uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
        display.drawPixel(x, y, WHITE);
      }
      break;
    }

    case DISPLAY_MODE::LINE :{
      
    }

    case DISPLAY_MODE::MOTOR :{
      printd(8,   24, "m4:"+to_string((int)motor_raw[3]) );
      printd(120, 24, "m3:"+to_string((int)motor_raw[2]), TEXT_ALIGN::RIGHT);
      printd(8,   56, "m1:"+to_string((int)motor_raw[0]) );
      printd(120, 56, "m2:"+to_string((int)motor_raw[1]), TEXT_ALIGN::RIGHT);

      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, move_dir, 24);

      break;
    }

    case DISPLAY_MODE::VARIABLES :{
      for(int i=0;i<debug_variables.size();i++){
        printd(16,24+8*i,debug_variables[i]);
      }
      break;
    }

    default:{
      printd(DISPLAY_W/2, DISPLAY_H/2, "internal error!", TEXT_ALIGN::CENTER, TEXT_ALIGN::MIDDLE);
      break;
    }

  }

  return;
}