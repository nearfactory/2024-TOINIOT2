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



  // 左端が反応した場合、0;5秒間は反応したものとし続ける
  // if(millis() - right_timer <= 500){
  //   return;
  // }



  // 壊れたセンサを反応しいないように修正
  line[0]    = false;
  line[14-1] = false;
  line[24-1] = false;

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

      // vec.x += cos(sensor_dir);
      // vec.y += sin(sensor_dir);

      num++;
    }
  }

  if(line[INNER_NUM]){
    vec2.x = 1.0f;
    num++;
  }
  if(line[INNER_NUM+1]){
    vec2.y = 1.0f;
    num++;
  }
  if(line[INNER_NUM+2]){
    vec1.x = -1.0f;
    num++;
  }
  if(line[INNER_NUM+3]){
    vec1.y = -1.0f;
    num++;
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

  // 踏み始め
  // if(prev_on == false && on == true){
  //   for(auto& q:queue) q = dir;
  // }
  // // 継続して踏んでいる場合
  // else if(prev_on == true && on == true){
  //   // 平均値のプラスマイナス45°を有効な範囲とする
  //   float range = 40.0f;
  //   float range_start = normalizeAngle(avr-range);
  //   float range_end   = normalizeAngle(avr+range);

  //   // 180°の壁をまたがない場合
  //   if(range_start <= range_end){
  //     if(dir <= range_start ||  range_end <= dir){
  //       dir = avr;
  //     }
  //   }else{
  //     if(range_end <= dir && dir <= range_start){
  //       dir = avr;
  //     }
  //   }
  // // 踏み終わり
  // }else if(prev_on == true && on == false){

  // }



  // if(line[INNER_NUM+3]){
  //   right_timer = millis();
  // }



  prev_on3 = prev_on2;
  prev_on2 = prev_on1;
  prev_on1 = on;

  // if(prev_on == true && on == false) dir = 0;



  queue[queue_i] = dir;
  queue_i = (queue_i+1) % QUEUE_SIZE;

  // 過去のベクトルの平均


  // float angle = 0;
  // float len = vec.len()*avr.len();
  // if(len!=0){
  //   angle = dotProduct(vec,avr) / len;
  //   angle = angle;
  // }

  // Serial.println(angle);

  // if(on){
  //   dir = degrees(-atan2(vec.y, vec.x));

  //   // 半分を超えても大丈夫なように
  //   // if(30<abs(dir) && abs(dir)<150){
  //   // }
  //   // if(dir*dir_prev < 0){
  //   //   dir += 180;
  //   //   if(dir>180) dir -= 360;
  //   // }
  //   distance = vec.len() / num;
  
  // // 内側が反応していない場合、外側を見る
  // }else{
  //   vec.clear();
  //   dir = 0;
  //   distance = 0;
    
  //   if(line[INNER_NUM]){
  //     on = true;
  //     vec.x += 1.0f;
  //   }
  //   if(line[INNER_NUM+1]){
  //     on = true;
  //     vec.y += 1.0f;
  //   }
  //   if(line[INNER_NUM+2]){
  //     on = true;
  //     vec.x += -1.0f;
  //   }
  //   if(line[INNER_NUM+3]){
  //     on = true;
  //     vec.y += -1.0f;
  //   }

  //   distance = vec.len();
  //   dir = degrees(atan2(vec.y, vec.x));
  // }

  // for(auto v:queue) Serial.printf("%f\t%f\t\t", v.x, v.y);
  // Serial.println();

  dir_prev = dir;
  vec_prev = vec;

  return;
}