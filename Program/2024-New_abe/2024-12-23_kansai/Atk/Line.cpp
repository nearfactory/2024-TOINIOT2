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



  // 不調のセンサを修正



  // 角度算出
  on = false;
  dir = 0;
  num = 0;
  vec1.set(10.0f, 10.0f);   // ちいさいx, y
  vec2.set(-10.0f, -10.0f); // でかいx, y

  // 最大値・最小値を算出  
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


  // ラインのベクトルを算出
  vec_prev = vec;

  float x = (vec1.x + vec2.x) / 2;
  float y = (vec1.y + vec2.y) / 2;
  vec.set(x, y);
  distance = vec.len();



  // サイドのセンサ
  outside = front | left | back | right;



  // 白線上かを判定
  prev_on = on;
  on = num > 0;


  // 角度算出
  dir = degrees(atan2(vec.y, vec.x));
  dir_prev = dir;



  // 踏み始め
  if(prev_on == false && on == true){
    dir_prev = dir;
  }

  // 継続して踏んでいる場合
  else if(prev_on == true && on == true){

    // 前回値の±90°を有効な範囲とする
    float range = 90.0;


    float range_start = normalizeAngle(dir_prev - range);
    float range_end   = normalizeAngle(dir_prev + range);
    

    // ±180°の壁をまたがない場合
    if(range_start <= range_end){
      if(dir <= range_start ||  range_end <= dir){
        dir = dir_prev;
      }
    }else{
      if(range_end <= dir && dir <= range_start){
        dir = dir_prev;
      }
    }

  }



  return;
}


void Line::send(char command){
  Serial1.print(command);
  return;
}
