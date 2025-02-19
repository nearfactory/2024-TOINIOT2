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
  num = 0;
  for(int i=0;i<6;i++){
    char c = Serial1.read();
    for(int j=0;j<5;j++){
      line[i*5+j] = (c>>(4-j)) & 0b00000001;
    }
  }



  // 壊れたセンサを反応しいないように修正
  line[2] = false;
  // for(auto l:line) Serial.printf("%d ", l);
  // Serial.println();
  


  // 調べる
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]) num++;
  }
  // 外側
  front = line[INNER_NUM];
  left  = line[INNER_NUM+1];
  back  = line[INNER_NUM+2];
  right = line[INNER_NUM+3];



  outside = front | left | back | right;

  angel = num > 0;
  on = angel | outside;
  // 踏んでいない場合に処理をスキップ
  if(!on){
    return;
  }



  // 角度算出
  vec.clear();



  // 初期化
  int index = 0;
  Vec2 v[INNER_NUM];
  int count[INNER_NUM];

  for(int i=0;i<INNER_NUM;i++){
    v[i].x = 0;
    v[i].y = 0;
    count[i] = 0;
  }


  // 連続する部分をひとまとめにする
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]){
      float sensor_dir = radians(i*360/INNER_NUM);
      v[index].x += cos(sensor_dir);
      v[index].y += sin(sensor_dir);
      count[index]++;
    }

    if(line[(i+INNER_NUM-1)%INNER_NUM] && !line[i]){
      index++;
    }
  }

  // ループの切れ目を処理
  if(line[INNER_NUM-1]){
    v[0].x += v[index].x;
    v[0].y += v[index].y;
  }

  area = index;

  
  // 合成
  for(int i=0;i<INNER_NUM;i++){
    // count[i]--;
    // if(count[i] < 1) count[i] = 1;
    if(count[i] != 0){
      v[i].x /= (float)count[i];
      v[i].y /= (float)count[i];
      vec.x += v[i].x;
      vec.y += v[i].y;
    }
  }


  // 外側のみ反応している場合
  if(!angel){
    if(front) vec.x = 1;
    if(left)  vec.y = 1;
    if(back)  vec.x = -1;
    if(right) vec.y = -1;
  }



  // 角度・距離を算出
  dir_prev = dir;
  dir = -degrees(atan2(vec.y, vec.x));
  distance = vec.len() / (float)index;


  // Serial.printf("dir:%f area:%d x:%f y:%f \n", dir, index, vec.x, vec.y);




  // 踏み始め
  if(prev_on == false && on == true){
    dir_prev = dir;
  }

  // 継続して踏んでいる場合
  else if(prev_on == true && on == true){

    // 前回値の±450°を有効な範囲とする
    float range = 22.5;


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
