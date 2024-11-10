// new atacker main program
// Teensy4.1
// 2024-09-11

#include <Wire.h>

#include "Ball.hpp"
#include "Motor.hpp"
#include "UI.hpp"

using namespace std;

void setup() {
  // 通信/ボールセンサ/方向センサ/キッカー/モーター/UIのセットアップ
  UISetup();
  ballSetup();
  motorSetup();  
}

void loop() {
  ballUpdate(BALL::DIR);
  if(!digitalRead(TOGGLE_PIN)){
    // データを更新
    
    // 回り込み (方法4)
    // https://yuta.techblog.jp/archives/40889399.html
    double r = 8000;  // 回り込み時の半径
    double theta = 0.0;
    if(abs(ball_dir)<15){
      move_dir = ball_dir * 1.5;
    }else if(ball_distance < r){
      theta = 90 + (r-ball_distance) * 90 / r;
      move_dir = ball_dir + (ball_dir>0?theta:-theta);
    }else{
      theta = asin(r/ball_distance);
      move_dir = ball_dir + (ball_dir>0?theta:-theta);
    } 
    moveDir(move_dir, 40, true, 100);
    
    // モーターに適用
    motorP();
    motorRaw();
  }else{
    motor[0] = 0;
    motor[1] = 0;
    motor[2] = 0;
    motor[3] = 0;
    
    motorP();
    motorRaw();
    delay(50);
  }
  display.clearDisplay();
  drawAngleLine(64, 32, ball_dir, 30);
  display.display();
}
