#pragma once

#include <string>
#include <vector>

#include <Arduino.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Ball.hpp"
extern Ball ball;
#include "Dir.hpp"
extern Dir dir;
#include "Line.hpp"
extern Line line;
#include "Motor.hpp"
extern Motor motor;
#include "UI.hpp"
// extern UI ui;

using namespace std;


enum class ALIGN : uint8_t{
  LEFT = 0,
  CENTER,
  RIGHT,
  
  TOP,
  MIDDLE,
  BOTTOM
};

enum MODE : uint8_t{
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


class Display{
private:
  // ディスプレイ本体
  static constexpr uint8_t DISPLAY_W = 128;
  static constexpr uint8_t DISPLAY_H = 64;
  static constexpr int8_t  DISPLAY_RESET = -1;
  static constexpr uint8_t DISPLAY_ADDR = 0x3c;
  
  Adafruit_SSD1306 display;

  // 

  // デバッグ用
  static constexpr uint8_t MODE_NUM = 9;
  vector<string> variables;
  vector<float*> valiables_addr;
  uint8_t mode = 0;
  uint8_t selector = 0;

public:
  Display() : display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET), variables(), mode(MODE::LINE) {}

  void begin();
  void clear();
  void draw();
  void next();

  void drawAngleLine(uint8_t cx, uint8_t cy, float angle, uint8_t r);
  void printd(uint8_t x, uint8_t y, std::string str, ALIGN align_x = ALIGN::LEFT, ALIGN align_y = ALIGN::TOP);

  void addValiables(std::string name, float* addr);


  void debug();
  void debug(uint8_t mode);
  void Ball();
  void Ble();
  void Camera();
  void Dir();
  void Dribbler();
  void Kicker();
  void Line();
  void Motor();
  void Valiables();
};

extern Display display;