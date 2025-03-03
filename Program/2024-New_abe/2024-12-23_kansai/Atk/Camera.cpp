#include "Camera.hpp"

void Camera::begin(){
  pixy.init();

  atk.sig = 2;
  def.sig = 1;

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



  if(atk.is_visible){
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

    // if(num_prev != atk.num){
    //   lock = false;
    // }

    // 敵避けの角度
    if(atk.num == 1){
      // 遠い方のカドにする
      if(lock){
        if(target == 0){
          chance_dir = x1_1;
        }else{
          chance_dir = x2_1;
        }
      }else{
        if(abs(atk.x1-160) > abs(atk.x2-160)){
          target = 0;
        }else{
          target = 1;
        }
      }
    }else{
      // int len_buf = 0, x1_buf = 0, x2_buf = 0;

      // 入れ替え
      // if(x1_1 > x1_2){
      //   len_buf = len_1;
      //   x1_buf  = x1_1;
      //   x2_buf  = x2_1;

      //   len_1 = len_2;
      //   x1_1  = x1_2;
      //   x2_1  = x2_2;

      //   len_2 = len_buf;
      //   x1_2  = x1_buf;
      //   x2_2  = x2_buf;
      // }

      if(lock){
        if(target == 0){
          chance_dir = x1_1;
        }else{
          chance_dir = x2_2;
        }
      }
      else{
        if(len_1 > len_2){
          if(x1_1 < x1_2){
            target = 0;
          }else{
            target = 1;
          }
        }else{
          if(x1_1 < x1_2){
            target = 1;
          }else{
            target = 0;
          }

        }
      }
    }

    chance_dir_prev = chance_dir;
    chance_dir = (chance_dir - 160) / 4.0;
    Serial.printf("num:%d x1_1:%d x2_1:%d len_1:%d x1_2:%d x2_2:%d len_2:%d target:%d chance_dir:%f lock:%d \n", atk.num, x1_1, x2_1, len_1, x1_2, x2_2, len_2, target, chance_dir, lock);
    lock = false;
  }


  if(def.is_visible){
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