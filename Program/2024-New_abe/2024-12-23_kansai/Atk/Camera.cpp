#include "Camera.hpp"

void Camera::begin(){
  pixy.init();

  atk.sig = 1;
  def.sig = 2;

  return;
}

void Camera::read(){
  pixy.ccc.getBlocks(false);

  block_num = pixy.ccc.numBlocks;
  if(block_num == 0){
    atk.is_visible = false;
    def.is_visible = false;
    return;
  }


  atk.is_visible = false;
  atk.num = 0;
  atk.x1 = 321;
  atk.y1 = 201;
  atk.x2 = -1;
  atk.y2 = -1;
  
  def.is_visible = false;
  def.num = 0;
  def.x1 = 321;
  def.y1 = 201;
  def.x2 = -1;
  def.y2 = -1;


  // 左上・右下座標を計算
  for(int i=0;i<block_num;i++){
    Block block;
    block.sig   = pixy.ccc.blocks[i].m_signature;
    block.x     = pixy.ccc.blocks[i].m_x;
    block.y     = pixy.ccc.blocks[i].m_y;
    block.w     = pixy.ccc.blocks[i].m_width;
    block.h     = pixy.ccc.blocks[i].m_height;
    block.angle = pixy.ccc.blocks[i].m_angle;
    block.index = pixy.ccc.blocks[i].m_index;
    block.age   = pixy.ccc.blocks[i].m_age;

    int x1 = block.x - block.w / 2;
    int x2 = block.x + block.w / 2;

    int y1 = block.y - block.h / 2;
    int y2 = block.y + block.h / 2;

    // atk
    if(block.sig == atk.sig){
      atk.is_visible = true;
      atk.num++;

      if(atk.x1 > x1) atk.x1 = x1;
      if(atk.x2 < x2) atk.x2 = x2;

      if(atk.y1 > y1) atk.y1 = y1;
      if(atk.y2 < y2) atk.y2 = y2;

    }
    // def
    else if(block.sig == def.sig){
      def.is_visible = true;
      def.num++;

      if(def.x1 > x1) def.x1 = x1;
      if(def.x2 < x2) def.x2 = x2;

      if(def.y1 > y1) def.y1 = y1;
      if(def.y2 < y2) def.y2 = y2;

    }

  }



  atk.x = (atk.x1 + atk.x2) / 2;
  atk.y = (atk.y1 + atk.y2) / 2;
  atk.w = atk.x2 - atk.x1;
  atk.h = atk.y2 - atk.y1;
  atk.dir_prev = atk.dir;

  atk.dir = (atk.x - 160) / 4.0;
  // atk.dir_queue[atk.dir_queue_id] = atk.dir;
  // float sum = 0;
  // for(auto d:atk.dir_queue) sum += d;
  // atk.dir = sum / (float) DIR_QUEUE_SIZE;
  // atk.dir_queue_id = (atk.dir_queue_id + 1) % DIR_QUEUE_SIZE;


  
  def.x = (def.x1 + def.x2) / 2;
  def.y = (def.y1 + def.y2) / 2;
  def.w = def.x2 - def.x1;
  def.h = def.y2 - def.y1;
  def.dir_prev = def.dir;

  def.dir = (def.x - 160) / 4.0;
  // def.dir_queue[def.dir_queue_id] = def.dir;
  // sum = 0;
  // for(auto d:def.dir_queue) sum += d;
  // def.dir = sum / (float)DIR_QUEUE_SIZE;
  // def.dir_queue_id = (def.dir_queue_id + 1) % DIR_QUEUE_SIZE;

  chance_dir_prev = chance_dir;
  chance_dir = atk.dir;

  return;
}

void Camera::changeAtk(){
  switch(atk.sig){
  case 1:
    atk.sig = 2;
    def.sig = 1;
    break;

  case 2:
    atk.sig = 1;
    def.sig = 2;
    break;

  default:
    atk.sig = 1;
    def.sig = 2;
    break;
  }

  return;
}