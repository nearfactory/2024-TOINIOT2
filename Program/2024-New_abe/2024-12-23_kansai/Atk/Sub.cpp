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

  // uint8_t str[4] = "";
  ball02k = Serial7.read();
  ball01k = Serial7.read();
  volume  = Serial7.read();
  // str[3]  = Serial7.read();

  // テスト
  // Serial.printf("min:0 max:255 b01k:%d b02k:%d vol:%d \n", str[0], str[1], str[2]);

  if(is_hold){
    if(ball01k > 130 || ball02k > 130){
      is_hold = false;
    }
  }else{
    if(ball01k < 100 || ball02k < 100){
      is_hold = true;
    }
  }

  return;
}


void Sub::kick(){
  if(millis()-kicked_timer > 10000){
    Serial7.print('k');
    kicked_timer = millis();
  }

  return;
}