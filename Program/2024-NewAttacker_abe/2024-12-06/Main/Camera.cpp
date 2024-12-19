#include "Camera.hpp"

void Camera::begin(){
  pixy.init();
  return;
}

void Camera::read(){
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks)
  {
    // 取得
    block.resize(pixy.ccc.numBlocks);
    x1 = 320;
    y1 = 200;
    x2 = 0;
    y2 = 0;

    for (int i=0; i<pixy.ccc.numBlocks; i++) {
      block[i].sig    = pixy.ccc.blocks[i].m_signature;
      block[i].x      = pixy.ccc.blocks[i].m_x;
      block[i].y      = pixy.ccc.blocks[i].m_y;
      block[i].width  = pixy.ccc.blocks[i].m_width;
      block[i].height = pixy.ccc.blocks[i].m_height;
      block[i].angle  = pixy.ccc.blocks[i].m_angle;
      block[i].index  = pixy.ccc.blocks[i].m_index;
      block[i].age    = pixy.ccc.blocks[i].m_age;

      if(x1 > block[i].x) x1 = block[i].x;
      if(y1 > block[i].y) y1 = block[i].y;
      if(x2 < block[i].x) x2 = block[i].x;
      if(y2 < block[i].y) y2 = block[i].y;
    }
  }else{
    block.resize(0);
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
  }

  x = 0;
  y = 0;
  dir = 0;


  return;
}