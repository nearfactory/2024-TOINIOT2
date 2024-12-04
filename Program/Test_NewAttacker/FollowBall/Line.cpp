#include "Line.hpp"

void Line::begin(int rate){
  baudrate = rate;
  Serial1.begin(baudrate);

  return;
}

void Line::read(){
  if(Serial1.available()<STR_SIZE){
    return;
  }

    
  // 古い情報を読み飛ばす
  while(Serial1.available()>STR_SIZE){
    Serial1.read();
  }
  // 開始コードが来るまで読み飛ばす
  while(Serial1.available()){
    if(Serial1.read() == 0b00011111) break;
  }

  // 送信中の場合、完了するまで待つ
  while(Serial1.available()<STR_SIZE-2){int i=0;}

  // 読み出して格納
  for(int i=0;i<6;i++){
    char c = Serial1.read();
    for(int j=0;j<5;j++){
      line[i*5+j] = (c>>(4-j)) & 0b00000001;
    }
  }

  // 壊れたセンサを反応しいないように修正
  line[0]    = false;
  line[14-1] = false;
  line[24-1] = false;

  // 角度算出
  on = false;
  num = 0;
  vec.clear();
  
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]){
      float sensor_dir = radians(i*360/INNER_NUM);
      
      vec.x += cos(sensor_dir);
      vec.y += sin(sensor_dir);

      num++;
      on = true;
    }
  }

  // 内側が反応していない場合、外側を見る
  if(!on){
    if(line[INNER_NUM]){
      on = true;
      dir = 0;
      distance = 1;
    }
    else if(line[INNER_NUM]){
      on = true;
      dir = -90;
      distance = 1;
    }
    else if(line[INNER_NUM]){
      on = true;
      dir = 180;
      distance = 1;
    }
    else if(line[INNER_NUM]){
      on = true;
      dir = 90;
      distance = 1;
    }
  }
  
  if(on){
    dir = degrees(-atan2(vec.y, vec.x));

    // 半分を超えても大丈夫なように
    // if(30<abs(dir) && abs(dir)<150){
    // }
    // if(dir*dir_prev < 0){
    //   dir += 180;
    //   if(dir>180) dir -= 360;
    // }
    distance = vec.len() / num;
  }else{
    dir = 0;
    distance = 0;
  }

  dir_prev = dir;

  return;
}