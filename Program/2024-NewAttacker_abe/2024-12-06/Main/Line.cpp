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

      // vec.x += cos(sensor_dir);
      // vec.y += sin(sensor_dir);

      num++;
      on = true;
    }
  }

  // ラインのベクトル
  float x = (vec1.x + vec2.x) / 2;
  float y = (vec1.y + vec2.y) / 2;
  vec.set(x, y);

  // 過去のベクトルの平均
  // Vec2 avr(0.0f, 0.0f);
  // for(auto v:queue) avr += v;
  // avr /= (float)QUEUE_SIZE;


  // float angle = 0;
  // float len = vec.len()*avr.len();
  // if(len!=0){
  //   angle = dotProduct(vec,avr) / len;
  //   angle = angle;
  // }

  // Serial.println(angle);

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
  
  // 内側が反応していない場合、外側を見る
  }else{
    dir = 0.0f;
    distance = 0.0f;
    
    if(line[INNER_NUM]){
      on = true;
      dir = 0.0f;
      distance = 1;
    }
    else if(line[INNER_NUM+1]){
      on = true;
      dir = -90.0f;
      distance = 1;
    }
    else if(line[INNER_NUM+2]){
      on = true;
      dir = 180.0f;
      distance = 1;
    }
    else if(line[INNER_NUM+3]){
      on = true;
      dir = 90.0f;
      distance = 1;
    }
  }

  queue[queue_i] = vec;
  queue_i = (queue_i+1) % QUEUE_SIZE;

  // for(auto v:queue) Serial.printf("%f\t%f\t\t", v.x, v.y);
  // Serial.println();

  dir_prev = dir;
  vec_prev = vec;

  return;
}