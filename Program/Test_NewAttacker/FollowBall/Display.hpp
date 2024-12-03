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
#include "UI.hpp"
// extern UI ui;

using namespace std;

class Display{
private:
  // ディスプレイ本体
  static constexpr uint8_t DISPLAY_W = 128;
  static constexpr uint8_t DISPLAY_H = 64;
  static constexpr int8_t  DISPLAY_RESET = -1;
  static constexpr uint8_t DISPLAY_ADDR = 0x3c;
  
  Adafruit_SSD1306 display;

  // 
  enum class ALIGN : uint8_t{
    LEFT = 0,
    CENTER,
    RIGHT,
    
    TOP,
    MIDDLE,
    BOTTOM
  };

  // デバッグ用
  static constexpr uint8_t MODE_NUM = 9;
  vector<string> variables;
  uint8_t mode = 0;
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

public:
  Display() : display(DISPLAY_W, DISPLAY_H, &Wire2, DISPLAY_RESET), variables() {}

  void begin();
  void clear();
  void draw();

  void drawAngleLine(uint8_t cx, uint8_t cy, float angle, uint8_t r);
  void printd(uint8_t x, uint8_t y, std::string str, ALIGN align_x = ALIGN::LEFT, ALIGN align_y = ALIGN::TOP);

  template <typename T>
  void add(std::string name, T variables);


  void debug(uint8_t mode);
  void Ball();
  void ble();
  void camera();
  void dir();
  void dribbler();
  void kicker();
  void line();
  void motor();
  void valiables();
};

extern Display display;