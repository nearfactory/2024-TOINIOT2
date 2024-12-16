#include "Camera.hpp"

using namespace std;

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
        block[i].x      = 320 - pixy.ccc.blocks[i].m_x;
        block[i].y      = pixy.ccc.blocks[i].m_y;
        block[i].width  = pixy.ccc.blocks[i].m_width;
        block[i].height = pixy.ccc.blocks[i].m_height;
        block[i].angle  = pixy.ccc.blocks[i].m_angle;
        block[i].index  = pixy.ccc.blocks[i].m_index;
        block[i].age    = pixy.ccc.blocks[i].m_age;
    }
    x = block[0].x;
    y = block[0].y;
    w = block[0].width;
    h = block[0].height;
  } 

  queue[queue_id] = h;
  queue_id = (queue_id+1)%10;
  int a = 0;
  for(auto q:queue) a+=q;
  h = a/10;

  return;
}