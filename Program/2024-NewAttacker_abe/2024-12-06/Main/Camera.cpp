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
    vector<Block> block(pixy.ccc.numBlocks);
    for (int i=0; i<pixy.ccc.numBlocks; i++) {
        block[i].sig    = pixy.ccc.blocks[i].m_signature;
        block[i].x      = pixy.ccc.blocks[i].m_x;
        block[i].y      = pixy.ccc.blocks[i].m_y;
        block[i].width  = pixy.ccc.blocks[i].m_width;
        block[i].height = pixy.ccc.blocks[i].m_height;
        block[i].angle  = pixy.ccc.blocks[i].m_angle;
        block[i].index  = pixy.ccc.blocks[i].m_index;
        block[i].age    = pixy.ccc.blocks[i].m_age;
    }
  } 

  return;
}