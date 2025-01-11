#pragma once

#include <vector>

#include <Arduino.h>

#include <Pixy2SPI_SS.h>

using namespace std;

// 機体の座標について
// コートが158x219(cm)
// -> x:-1~1, y:-1.4~1.4

// 出力値
// 横:320 縦:200
  
class Camera{
private:
  Pixy2SPI_SS pixy;

  using Block = struct{
    uint16_t sig=0;
    uint16_t x=0;
    uint16_t y=0;
    uint16_t w=0;
    uint16_t h=0;
    int16_t angle=0;
    uint8_t index=0;
    uint8_t age=0;
  };

  static constexpr uint8_t  DIR_QUEUE_SIZE = 10;

  using Goal = struct{
    bool is_visible = false;
    int  sig = 1;

    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;

    int x = 0, y = 0;
    int w = 0, h = 0;
    int num = 0;

    float dir = 0;
    float dir_prev = 0;

    int   dir_queue_id = 0;
    float dir_queue[DIR_QUEUE_SIZE]{};
  };

public:
  // Pixyからのデータ
  int           block_num = 0;

  Goal atk, def;
  float chance_dir = 0; // シュートコース(ゴールの空いている方の角)の角度
  float chance_dir_prev = 0;

  void begin();
  void read();
  void changeAtk();
};

extern Camera camera;