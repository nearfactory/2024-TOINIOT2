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



  // キューにコピー
  for(int i=0;i<LINE_NUM;i++){
    binary_queue[binary_queue_id][i] = line[i];
  }
  binary_queue_id = ( binary_queue_id + 1 ) % BINARY_QUEUE_SIZE;

  for(int j=0;j<BINARY_QUEUE_SIZE;j++){
    for(int i=0;i<LINE_NUM;i++){
      line[i] |= binary_queue[j][i];
    }
  }

  // 角度算出
  on = false;
  dir = 0;
  num = 0;
  vec1.set(10.0f, 10.0f);   // ちいさいx, y
  vec2.set(-10.0f, -10.0f); // でかいx, y
  
  /*
  // ベクトルのxy成分それぞれの最大値・最小値
  for(int j=0;j<BINARY_QUEUE_SIZE;j++){
    for(int i=0;i<INNER_NUM;i++){
      if(binary_queue[j][i]){
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
  }

  for(int j=0;j<BINARY_QUEUE_SIZE;j++){
    if(binary_queue[j][INNER_NUM]){
      vec2.x = 1.0f;
      num++;
    }
    if(binary_queue[j][INNER_NUM+1]){
      vec2.y = 1.0f;
      num++;
    }
    if(binary_queue[j][INNER_NUM+2]){
      vec1.x = -1.0f;
      num++;
    }
    if(binary_queue[j][INNER_NUM+3]){
      vec1.y = -1.0f;
      num++;
    }
  }
  */

  
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


  if(num == 0){
    if(line[INNER_NUM]){
      vec2.x = 1.0f;
      front = true;
      num++;
    }
    if(line[INNER_NUM+1]){
      vec2.y = 1.0f;
      left = true;
      num++;
    }
    if(line[INNER_NUM+2]){
      vec1.x = -1.0f;
      back = true;
      num++;
    }
    if(line[INNER_NUM+3]){
      vec1.y = -1.0f;
      right = true;
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



  // 過去の角度と比較し、45度以上の差があれば無効とする
  float avr = 0;
  for(auto q:queue) avr += q;
  avr /= (float)QUEUE_SIZE;

  bool prev_on = prev_on1 | prev_on2 | prev_on3;
  prev_on3 = prev_on2;
  prev_on2 = prev_on1;
  prev_on1 = on;

  // 踏み始め
  if(prev_on == false && on == true){
    for(auto& q:queue) q = dir;
    dir_prev = dir;
  }
  // 継続して踏んでいる場合
  else if(prev_on == true && on == true){
    // 平均値のプラスマイナス45°を有効な範囲とする
    float range = 30.0;
    // float range_start = normalizeAngle(avr-range);
    // float range_end   = normalizeAngle(avr+range);

    float range_start = normalizeAngle(dir_prev - range);
    float range_end   = normalizeAngle(dir_prev + range);
    
    // 180°の壁をまたがない場合
    if(range_start <= range_end){
      if(dir <= range_start ||  range_end <= dir){
        dir = dir_prev;
      }
    }else{
      if(range_end <= dir && dir <= range_start){
        dir = dir_prev;
      }
    }
  // 踏み終わり
  }else if(prev_on == true && on == false){
    on = true;
    dir = dir_prev;
  }else if(prev_on == false && on == false){
    on = false;
    dir = 0;
  }

  queue[queue_i] = dir;
  queue_i = (queue_i+1) % QUEUE_SIZE;

  dir_prev = dir;
  vec_prev = vec;

  return;
}

void Line::send(char command){
  Serial1.print(command);
  return;
}
