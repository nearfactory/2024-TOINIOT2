#include "Sub.hpp"

void Sub::begin(){
  Serial7.begin(115200);
  return;
}

void Sub::read(){
  // 必要な分のデータを受信していない場合処理を飛ばす
  if(Serial7.available()<STR_SIZE){
    return;
  }
    
  // // 古い情報を読み飛ばす
  while(Serial7.available()>STR_SIZE){
    Serial7.read();
  }

  uint8_t str[4] = "";
  str[0] = Serial7.read();
  str[1] = Serial7.read();
  str[2] = Serial7.read();
  str[3] = Serial7.read();

  // テスト
  // Serial.printf("min:0 max:255 b01k:%d b02k:%d vol:%d \n", str[0], str[1], str[2]);

  int sum1 = 0;
  int sum2 = 0;
  for(int i=0;i<QUEUE_SIZE;i++){
    sum1 += queue[i][0];
    sum2 += queue[i][1];
  }
  ball01k = sum1 / QUEUE_SIZE;
  ball02k = sum2 / QUEUE_SIZE;

  if(is_hold){
    if(ball01k > 130 || ball02k > 130){
      is_hold = false;
    }
  }else{
    if(ball01k < 80 || ball02k < 80){
      is_hold = true;
    }
  }

  return;
}

void kick(){
  Serial7.print('k');
  return;
}