#pragma once

#include <string>
#include <vector>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
// #include "Dir.hpp"
#include "Kicker.hpp"

using namespace std;

namespace{
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 36,37,31,30 };

  constexpr uint8_t BZ_PIN = 33;

  constexpr uint8_t DISPLAY_W = 128;
  constexpr uint8_t DISPLAY_H = 64;
  constexpr uint8_t DISPLAY_RESET = -1;
  constexpr uint8_t DISPLAY_ADDR = 0x3c;
  constexpr uint8_t DISPLAY_MODE_NUM = 9;
  const std::string DISPLAY_MODE_NAME[DISPLAY_MODE_NUM] = {
    "ball",
    "ball-k",
    "ble",
    "camera",
    "dir",
    "dribbler",
    "line",
    "motor",
    "variables"
  };

}
enum DISPLAY_MODE : uint8_t{
  BALL = 0,
  BALL_K,
  BLE,
  CAMERA,
  DIR,
  DRIBBLER,
  LINE,
  MOTOR,
  VARIABLES
};
enum class TEXT_ALIGN_X : uint8_t{
  LEFT = 0,
  CENTER,
  RIGHT
};
enum class TEXT_ALIGN_Y : uint8_t{
  TOP = 0,
  MIDDLE,
  BOTTOM
};


bool                button[BUTTON_NUM] = {false};
bool                previous_button[BUTTON_NUM] = {false};
float               bz = -1.0f;
Adafruit_SSD1306    display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET);
uint8_t             DISPLAY_MODE = 0;
vector<std::string> debug_variables(0);


inline void UISetup(){
  // button
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);

  // buzzer
  pinMode(BZ_PIN, OUTPUT);

  // display
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)){
    Serial.println("display error!");
    while(true);
  }
  display.setTextColor(WHITE);
  // display.setTextSize(1);
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
  num = num<1 ? 1 : num;
  num = num>5 ? 5 : num;
  return (!button[num-1]) && previous_button[num-1];
}

// buzzer
inline void bzUpdate(){
  if(bz > -1.0f){
    tone(BZ_PIN, bz);
  }
  return;
}

// display
inline void printd(uint8_t x, uint8_t y, std::string str, TEXT_ALIGN_X align_x = TEXT_ALIGN_X::LEFT, TEXT_ALIGN_Y align_y = TEXT_ALIGN_Y::TOP){
  switch(align_x){
    case TEXT_ALIGN_X::LEFT :
      break;
    case TEXT_ALIGN_X::CENTER :
      x -= str.length()*8/2;
      break;
    case TEXT_ALIGN_X::RIGHT :
      x -= str.length()*8;
      break;
  }
  switch(align_y){
    case TEXT_ALIGN_Y::TOP :
      break;
    case TEXT_ALIGN_Y::MIDDLE :
      y -= 8/2;
      break;
    case TEXT_ALIGN_Y::BOTTOM :
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
}

template <typename T>
inline void addVariables(std::string name, T variables){
  debug_variables.emplace_back(name+":"+to_string(variables));
  return;
}

inline void debugDisplay(uint8_t mode){
  printd(8,8,DISPLAY_MODE_NAME[mode]);
  // printd(120,32,"4.mode", TEXT_ALIGN_X::RIGHT, TEXT_ALIGN_Y::MIDDLE);

  switch(mode){
    case DISPLAY_MODE::BALL :{
      uint8_t circle_r = 24;
      uint8_t text_r = circle_r + 4;
      
      string str = to_string(ball_dir);
      str.erase(str.begin()+5,str.end());
      printd(8, 32, str, TEXT_ALIGN_X::LEFT, TEXT_ALIGN_Y::MIDDLE);
      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, -ball_dir, circle_r);
      
      double ball_small_angle = (ball_small_id*360.0/BALL_NUM+180.0)/180.0*3.14;
      printd(DISPLAY_W/2+text_r*cos(-ball_small_angle), DISPLAY_H/2+text_r*sin(-ball_small_angle), to_string(ball_small), TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      double ball_big_angle = (ball_big_id*360.0/BALL_NUM+180.0)/180.0*3.14;
      printd(DISPLAY_W/2+text_r*cos(-ball_big_angle), DISPLAY_H/2+text_r*sin(-ball_big_angle), to_string(ball_big), TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);

      for(int i=0;i<BALL_NUM;i++){
        double angle = (i*360/BALL_NUM+180);
        // Serial.println(angle);
        angle = angle/180*3.14;
        uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
        uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
        // printd(x, y, ".", TEXT_ALIGN_X::LEFT, TEXT_ALIGN_Y::BOTTOM);
        display.drawPixel(x, y, WHITE);
        // printd(DISPLAY_W/2+cos(angle)*circle_r, DISPLAY_H/2+sin(angle)*circle_r, to_string(ball[i]),TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      }
      break;
    }

    case DISPLAY_MODE::BALL_K :{
      printd(64,32,"no data", TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      break;
    }

    case DISPLAY_MODE::BLE :{
      printd(8, 24, "ATK:"+BLE_atk);
      printd(8, 40, "DEF:"+BLE_def);
      break;
    }

    case DISPLAY_MODE::CAMERA :{
      printd(64,32,"no data", TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      break;
    }

    case DISPLAY_MODE::DIR :{
      string str = to_string(dir);
      str.erase(str.begin()+5,str.end());
      printd(8, 32, str, TEXT_ALIGN_X::LEFT, TEXT_ALIGN_Y::MIDDLE);

      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, -default_dir, 16);
      drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, -dir, 24);
      break;
    }
    
    case DISPLAY_MODE::DRIBBLER :{
      printd(64,32,"no data", TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      break;
    }

    case DISPLAY_MODE::LINE :{
      printd(64,32,"no data", TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      break;
    }

    case DISPLAY_MODE::MOTOR :{
      printd(8,   24, "m4:"+to_string(motor[3]) );
      printd(120, 24, "m3:"+to_string(motor[2]), TEXT_ALIGN_X::RIGHT);
      printd(8,   56, "m1:"+to_string(motor[0]) );
      printd(120, 56, "m2:"+to_string(motor[1]), TEXT_ALIGN_X::RIGHT);

      printd(120,32,"stop",TEXT_ALIGN_X::RIGHT,TEXT_ALIGN_Y::MIDDLE);
      buttonUp(3);
      break;
    }

    case DISPLAY_MODE::VARIABLES :{
      // printd(64,32,"no data", TEXT_ALIGN_X::CENTER, TEXT_ALIGN_Y::MIDDLE);
      for(int i=0;i<debug_variables.size();i++){
        printd(8,16+8*i,debug_variables[i]);
      }
      break;
    }

    default:{
      for(int i=0;i<debug_variables.size();i++){
        printd(8,24,debug_variables[i]);
      }
      printd(8,8,"internal error!");
      break;
    }

  }

  return;
}