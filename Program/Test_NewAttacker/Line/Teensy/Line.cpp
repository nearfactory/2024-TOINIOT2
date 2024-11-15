#include "Line.hpp"

void Line::Begin(int rate){
  this->baudrate = rate;
  Serial1.begin(baudrate);

  return;
}

void Line::Read(){
  // 古い情報を読み飛ばす
  while(Serial1.available()>str_size){
    Serial1.read();
  }
  // 開始コードが来るまで読み飛ばす
  while(Serial1.available()){
    if(Serial1.read() == 0b00011111) break;
  }

  // 送信中の場合、完了するまで待つ
  while(Serial1.available()<=str_size-3){int i=0;}

  // 読み出して格納
  for(int i=0;i<6;i++){
    char c = Serial1.read();
    for(int j=0;j<5;j++){
      line[i*5+j] = (c>>(4-j)) & 0b00000001;
    }
  }

  // 壊れたセンサを反応しいないように修正
  line[0] = false;
  line[14-1] = false;
  line[24-1] = false;

  Vec2  vec(0.0f, 0.0f);
  for(int i=0;i<LINE_INNER_NUM;i++){
    if(line[i]){
      float sensor_dir = radians(i*360/LINE_INNER_NUM);
      vec.x += cos(sensor_dir);
      vec.y += sin(sensor_dir);
      line_num++;
    }
  }
  line_dir = -atan2(line_vec.y, line_vec.x);
}