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



  // 角度算出
  num = 0;
  vec.clear();
  vec1.set(10.0f, 10.0f);   // ちいさいx, y
  vec2.set(-10.0f, -10.0f); // でかいx, y



  int index = 0;
  Vec2 v[INNER_NUM];
  int count[INNER_NUM];

  for(int i=0;i<INNER_NUM;i++){
    v[i].x = 0;
    v[i].y = 0;
    count[i] = 0;
  }


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

  if(line[INNER_NUM-1]){
    v[0].x += v[index].x;
    v[0].y += v[index].y;
  }

  for(int i=0;i<INNER_NUM;i++){
    // Serial.printf("x:%f y:%f ", v[i].x, )
    if(count[i] != 0){
      v[i].x /= (float)count[i];
      v[i].y /= (float)count[i];
      vec.x += v[i].x;
      vec.y += v[i].y;
    }
  }



  // ラインのベクトル
  // float x = (vec1.x + vec2.x) / 2;
  // float y = (vec1.y + vec2.y) / 2;
  // vec.set(x, y);

  dir = -degrees(atan2(vec.y, vec.x));
  distance = vec.len() / (float)(index+1);

  Serial.printf("dir:%f area:%d x:%f y:%f \n", dir, index, vec.x, vec.y);



  // 白線上か  
  on = num > 0;



  // 外側
  outside = front | left | back | right;



  return;

}


void Line::send(char command){
  Serial1.print(command);
  return;
}
