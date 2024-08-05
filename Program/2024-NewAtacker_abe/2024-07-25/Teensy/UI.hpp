#pragma once

#include <string>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
// #include "Dir.hpp"
#include "Kicker.hpp"

namespace{
  constexpr uint8_t BUTTON_NUM = 4;
  const     uint8_t BUTTON_PIN[BUTTON_NUM] = { 36,37,31,30 };

  constexpr uint8_t BZ_PIN = 33;

  constexpr uint8_t DISPLAY_W = 128;
  constexpr uint8_t DISPLAY_H = 64;
  constexpr uint8_t DISPLAY_RESET = -1;
  constexpr uint8_t DISPLAY_ADDR = 0x3c;

}
uint8_t MODE = 0;
constexpr uint8_t MODE_NUM = 8;
const std::string MODE_NAME[MODE_NUM] = {
  "ball",
  "ball-k",
  "ble",
  "camera",
  "dir",
  "dribbler",
  "line",
  "valiables"
};
enum MODE : uint8_t{
  BALL = 0,
  BALL_K,
  BLE,
  CAMERA,
  DIR,
  DRIBBLER,
  LINE,
  VALIABLES
};


bool button[BUTTON_NUM] = {false};
bool previous_button[BUTTON_NUM] = {false};
float bz = -1.0f;
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET);


void UISetup(){
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

void buttonUpdate(){
  for(int i=0;i<BUTTON_NUM;i++){
    previous_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

bool buttonUp(uint8_t num){
  num = num<1 ? 1 : num;
  num = num>5 ? 5 : num;
  return (!button[num-1]) && previous_button[num-1];
}

void bzUpdate(){
  if(bz > -1.0f){
    tone(BZ_PIN, bz);
  }
  return;
}

void printd(uint8_t x, uint8_t y, std::string str){
  display.setCursor(x, y);
  display.print(str.c_str());

  return;
}

void drawAngleLine(float angle, uint8_t r){
  display.drawLine(DISPLAY_W/2, DISPLAY_H/2, DISPLAY_W/2+cos(angle*3.14/180)*r, DISPLAY_H/2+sin(angle*3.14/180)*r, WHITE);

  return;
}

void debugDisplay(uint8_t mode){
    // 0 "ball",
    // 1 "ball-k",
    // 2 "ble",
    // 3 "camera",
    // 4 "dir",
    // 5 "dribbler",
    // 6 "line",
    // 7 "valiables"
  printd(8,8,MODE_NAME[mode]);

  switch(mode){
    case 0:
      drawAngleLine(-ball_dir,24);
      break;
    case 1:
      printd(64,32,"no data");
      break;
    case 2:
      printd(8, 24, "ATK:"+BLE_atk);
      printd(8, 40, "DEF:"+BLE_def);
      break;
    case 3:
      printd(64,32,"no data");
      // display.setCursor(64,32);
      // display.print("no data");
      break;
    case 4:
      drawAngleLine(-dir, 24);
      break;
    case 5:
      printd(64,32,"no data");
      break;
    case 6:
      printd(64,32,"no data");
      break;
    case 7:
      printd(64,32,"no data");
      break;
    default:
      printd(8,8,"internal error!");
      break;
  }

  return;
}