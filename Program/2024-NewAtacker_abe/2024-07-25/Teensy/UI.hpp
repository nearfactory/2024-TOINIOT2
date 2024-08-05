#pragma once

#include <string>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

namespace{
  constexpr uint8_t BTN_NUM = 4;
  const     uint8_t BTN_PIN[BTN_NUM] = { 36,37,31,30 };

  constexpr uint8_t BZ_PIN = 33;

  constexpr uint8_t DISPLAY_W = 128;
  constexpr uint8_t DISPLAY_H = 64;
  constexpr uint8_t DISPLAY_RESET = -1;
  constexpr uint8_t DISPLAY_ADDR = 0x3c;
}


bool btn[BTN_NUM] = {false};
float bz = -1.0f;
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET);


void UISetup(){
  // button
  for(auto p:BTN_PIN) pinMode(p, INPUT);

  // buzzer
  pinMode(BZ_PIN, OUTPUT);

  // display
  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)){
    Serial.println("display error!");
    while(true);
  }
  return;
}

void btnUpdate(){
  for(int i=0;i<BTN_NUM;i++){
    btn[i] = digitalRead(BTN_PIN[i]);
  }
  return;
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