#include "Camera.hpp"

void Camera::begin(){
  pixy.init();
  return;
}

void Camera::read(){
  pixy.ccc.getBlocks(false);

  x = 0;
  y = 0;
  dir = 0;

  block_num = pixy.ccc.numBlocks;
  if(block_num){
    // 取得
    block.resize(pixy.ccc.numBlocks);

    for (int i=0; i<pixy.ccc.numBlocks; i++) {
      block[i].sig    = pixy.ccc.blocks[i].m_signature;
      block[i].x      = pixy.ccc.blocks[i].m_x - pixy.ccc.blocks[i].m_width/2;
      block[i].y      = pixy.ccc.blocks[i].m_y - pixy.ccc.blocks[i].m_height/2;
      block[i].width  = pixy.ccc.blocks[i].m_width;
      block[i].height = pixy.ccc.blocks[i].m_height;
      block[i].angle  = pixy.ccc.blocks[i].m_angle;
      block[i].index  = pixy.ccc.blocks[i].m_index;
      block[i].age    = pixy.ccc.blocks[i].m_age;

    }

    prev_goal_dir = goal_dir;
    atk_num = 0;

    x1 = 320;
    y1 = 200;
    x2 = 0;
    y2 = 0;
    for(int i=0;i<pixy.ccc.numBlocks;i++){
      // 黄色(sig=1)のみ処理を行う
      if(block[i].sig == atk_sig){
        int _x1 = block[i].x;
        int _y1 = block[i].y;
        int _x2 = block[i].x+block[i].width;
        int _y2 = block[i].y+block[i].height;
        if(x1 > _x1) x1 = _x1;
        if(y1 > _y1) y1 = _y1;
        if(x2 < _x2) x2 = _x2;
        if(y2 < _y2) y2 = _y2;

        atk_num++;
      }
    }

    atk_x = (x1+x2) / 2 - 160;
    atk_y = (y1+y2) / 2;
    atk_w = x2-x1;
    atk_h = y2-y1;


    goal_dir = (float)atk_x / 4.0;

    dir_queue[dir_queue_id] = goal_dir;
    dir_queue_id = (dir_queue_id+1)%DIR_QUEUE_SIZE;

    // ゴールの角度を算出
    float buf = 0;
    for(auto q:dir_queue) buf += q;
    goal_dir = buf / (float)DIR_QUEUE_SIZE;
  }else{
    block.resize(0);
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
  }


  

  return;
}

void Camera::changeAtk(){
  switch(atk_sig){
  case 1:
    atk_sig = 2;
    break;
  case 2:
    atk_sig = 1;
    break;
  default:
    atk_sig = 1;
    break;
  }
  return;
}