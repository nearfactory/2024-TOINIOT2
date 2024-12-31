#include "Line.hpp"

void Line::begin(int rate){
  baudrate = rate;
  Serial1.begin(baudrate);

  return;
}

void Line::read(){
  // 必要な分のデータを受信していない場合処理を飛ばす
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

  line[24]   = false;   // 不安定



  // 角度算出
  on = false;
  dir = 0;
  num = 0;
  vec1.set(10.0f, 10.0f);   // ちいさいx, y
  vec2.set(-10.0f, -10.0f); // でかいx, y
  
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]){
      float sensor_dir = radians(i*360/INNER_NUM);

      float x = cos(sensor_dir);
      float y = sin(sensor_dir);
      
      if(vec1.x > x) vec1.x = x;
      if(vec2.x < x) vec2.x = x;

      if(vec1.y > y) vec1.y = y;
      if(vec2.y < y) vec2.y = y;

      num++;
    }
  }

  

  // ラインのベクトル
  float x = (vec1.x + vec2.x) / 2;
  float y = (vec1.y + vec2.y) / 2;
  vec.set(x, y);
  
  on = num > 0;
  distance = vec.len();
  dir = degrees(atan2(vec.y, vec.x));



  dir_prev = dir;
  vec_prev = vec;

  return;
}

void Line::send(char command){
  Serial1.print(command);
  return;
}
