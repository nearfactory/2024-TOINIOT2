#include "Camera.hpp"

void Camera::begin(){
  pixy.init();

  atk.sig = 1;
  def.sig = 2;

  return;
}

void Camera::read(){
  // 取得
  pixy.ccc.getBlocks(false);

  // どちらのゴールも見えていない場合は終了
  block_num = pixy.ccc.numBlocks;
  if(block_num == 0){
    atk.is_visible = false;
    def.is_visible = false;
    return;
  }



  // 左右反転処理
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    pixy.ccc.blocks[i].m_x = 320 - pixy.ccc.blocks[i].m_x;
  }



  // バウンディングボックスの計算
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

  x1_1 = 0;
  x2_1 = 0;
  len_1 = 0;
  x1_2 = 0;
  x2_2 = 0;
  len_2 = 0;

  // chance.area = 0;
  // chance.area.x1 = 321;
  // chance.is_left = false; // 左側のブロックであるか？


  int count = 0;
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


      if(count == 0){
        x1_1 = x1;
        x2_1 = x2;
        len_1 = x2 - x1;
      }else if(count == 1){
        x1_2 = x1;
        x2_2 = x2;
        len_2 = x2 - x1;
      }

      count++;

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


  // 敵避け用
  // [x1] -- [x2]
  if(atk.is_visible && atk.num == 2){
    enemy.is_visible = true;
    
    int x1_1 = pixy.ccc.blocks[0].m_x - pixy.ccc.blocks[0].m_width / 2;
    int x2_1 = pixy.ccc.blocks[0].m_x + pixy.ccc.blocks[0].m_width / 2;
    
    int x1_2 = pixy.ccc.blocks[1].m_x - pixy.ccc.blocks[1].m_width / 2;
    int x2_2 = pixy.ccc.blocks[1].m_x + pixy.ccc.blocks[1].m_width / 2;

    // 入れ替え
    if(x1_1 > x1_2){
      int b = x1_1;
      x1_1 = x1_2;
      x1_2 = b;

      b = x2_1;
      x2_1 = x2_2;
      x2_2 = b;
    }

    enemy.x1 = x2_1;
    enemy.x2 = x1_2;

    enemy.w = enemy.x2 - enemy.x1;
  }else{
    enemy.is_visible = false;
  }



  if(atk.is_visible){
    atk.x = (atk.x1 + atk.x2) / 2;
    atk.y = (atk.y1 + atk.y2) / 2;
    atk.w = atk.x2 - atk.x1;
    atk.h = atk.y2 - atk.y1;
    atk.dir_prev = atk.dir;

    atk.dir = (atk.x - 160) / 4.0;

    if(atk.x1 < 160 && 160 < atk.x2){
      is_center = true;
    }else{
      is_center = false;
    }
  }


  if(def.is_visible){
    def.x = (def.x1 + def.x2) / 2;
    def.y = (def.y1 + def.y2) / 2;
    def.w = def.x2 - def.x1;
    def.h = def.y2 - def.y1;
    def.dir_prev = def.dir;

    def.dir = (def.x - 160) / 4.0;
  }



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