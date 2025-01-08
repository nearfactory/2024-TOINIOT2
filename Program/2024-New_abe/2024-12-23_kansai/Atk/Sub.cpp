#include "HardwareSerial.h"
#include "Sub.hpp"

void Sub::begin(){
  Serial7.begin(115200);
  return;
}


void Sub::read(){
  // 必要な分のデータを受信していない場合処理を飛ばす
  // if(Serial7.available()<STR_SIZE+1){
  //   return;
  // }
   
  // // 古い情報を読み飛ばす
  while(Serial7.available()>STR_SIZE*2){
    Serial7.read();
  }
   
  while(Serial7.available()){
    if(Serial7.read() == '\0') break;
  }

  while(Serial7.available()<4){int i=0;}

  ball02k = Serial7.read();
  ball01k = Serial7.read();
  volume  = Serial7.read();

  // テスト
  // Serial.printf("min:0 max:255 b01k:%d b02k:%d vol:%d \n", str[0], str[1], str[2]);


  // 保持判定
  brightness = ball01k;
  is_hold = brightness < THRESHOLD;


  // キッカーの準備判定
  ready = millis()-kicked_timer > INTERVAL;

  return;
}


void Sub::kick(){
  ready = millis()-kicked_timer > INTERVAL;

  if(ready){
    Serial7.print('k');
    kicked_timer = millis();
  }

  return;
}