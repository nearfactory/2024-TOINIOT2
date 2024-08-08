#include "pin.h"
#include "motor.h"
#include "tone.h"

int mode = 0;

void setup() {
  pinSetup();
  motorSetup(500, 100, 100, 100, 100);
  motorGainSetup(1, 1, 1);

  tone(bzPin, soundPitch[9]);
  waitBtn(0);
  noTone(bzPin);
}

void loop() {
  // 直近に押されたボタンの種類によってモードを変更する
  if(digitalRead(btnPin[0]) == 1){
    mode = 0;
  }
  if(digitalRead(btnPin[1]) == 1){
    mode = 1;
  }
  if(digitalRead(btnPin[2]) == 1){
    mode = 2;
  }

  // モードによって計算するモーターパワーの目標値を変更して処理
  switch(mode){
  case 0:
    mpCalc(100, 100, -100, -100);
    break;
  
  case 1:
    mpCalc(-100, -100, 100, 100);
    break;

  case 2:
    mpCalc(100, -100, -100, 100);
    break;
  }
  
  // モーターへ出力
  move();

  // モーターパワーをシリアル通信で出力
  for(int i=0; i<4; i++){
    Serial.print(mpOutput[i]);
    Serial.print("\t");
  }
  Serial.print(100);
  Serial.print("\t");
  Serial.println(-100);
}