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

  static constexpr uint8_t  DIR_QUEUE_SIZE = 10;
  uint8_t                   dir_queue_id = 0;
  float                     dir_queue[DIR_QUEUE_SIZE]{};

  struct Block{
    uint16_t sig=0;
    uint16_t x=0;
    uint16_t y=0;
    uint16_t width=0;
    uint16_t height=0;
    int16_t angle=0;
    uint8_t index=0;
    uint8_t age=0;
  };
public:
  // Pixyからのデータ
  vector<Block> block;
  int           block_num = 0;


  // 自己位置推測
  float x = 0;
  float y = 0;
  float dir = 0;
  float prev_dir = 0;

  int atk_sig = 1;
  int atk_x = 0;
  int atk_y = 0;
  int atk_w = 0;
  int atk_h = 0;
  int atk_num = 0;
  
  float goal_dir = 0;

  // 画像上
  int x1 = 0, y1 = 0;
  int x2 = 0, y2 = 0;

  void begin();
  void read();
  void changeAtk();
};

extern Camera camera;