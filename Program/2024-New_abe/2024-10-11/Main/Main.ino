// new atacker main program
// Teensy4.1
// 2024-09-11

// PCを再起動して、リセットボタンを押しながらusbを指す

#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
#include "Dir.hpp"
#include "Kicker.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

using namespace std;

void setup() {
  // 通信/ボールセンサ/方向センサ/キッカー/モーター/UIのセットアップ
  communicationSetup();
  ballSetup();
  dirSetup();
  kickerSetup();
  lineSetup();
  motorSetup();
  UISetup();

  // BNO055のキャリブレーション
  /*
  ・必ず試合開始までに済ませる
  ・ロボットを回すときに落とさないように注意
  ・一通り回した後は床などの動かない場所（コート上は×）にて安定させる
  ・キャリブレーション完了後に再度少し水平に回転・安定させる
  ・ディスプレイに表示される角度が放置しても変わらなくなったらキャリブレーション完了
  */

  display_mode = DISPLAY_MODE::DIR;
  // ディスプレイモードを方向センサ用に変更
  // キャリブレーションの状況変数を初期化 
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  system = 3; gyro = 3; mag = 3;
  // キャリブレーション完了まで繰り返し
  while(system<3 || gyro<3 || mag<3){
    display.clearDisplay();
    printd(8,8,"calibration...");
    Serial.println("calibration...");

    // キャリブレーション状況を取得・ディスプレイバッファへ書き込み
    bno.getCalibration(&system, &gyro, &accel, &mag);
    printd(32,24,"system:"+to_string(system));
    printd(32,32,"gyro  :"+to_string(gyro));
    printd(32,40,"accel :"+to_string(accel));
    printd(32,48,"mag   :"+to_string(mag));

    // ディスプレイを更新
    display.display();
    delay(10);
  }

  // スタート画面を表示
  display.clearDisplay();
  printd(8,8,"start",TEXT_ALIGN::RIGHT, TEXT_ALIGN::BOTTOM);
  display.display();

  Serial.println("waiting...");
  while(digitalRead(TOGGLE_PIN) == 0){ Serial.println("off"); delay(50);}
  while(digitalRead(TOGGLE_PIN) == 1){ Serial.println("on");  delay(50);}

  Serial.println("beging");

  // delay(500);
  // 攻め方向を取得・更新
  /*
  */
  sensors_event_t d{};
  bno.getEvent(&d, Adafruit_BNO055::VECTOR_EULER);
  default_dir = d.orientation.x;
  
  // delay(5000);
}

void loop() {

  // データを更新
  ballUpdate(BALL::DIR);
  dirUpdate();
  // lineUpdate();
  // subUpdate();
  
  // 回り込み
  // moveDir(ball_dir, 20, true, 100);

  // ボールを保持している
  if(ball_holding) {
    moveDir(0, 100, true, 100);
  }

  // 白線避け
  avoidLine();

  // 姿勢制御
  setDir(dir,0,100.0,20);

  // ボールが存在しない
  if(!ball_exist) setDir(dir,0,100.0,100);
  
  // display
  if(!digitalRead(TOGGLE_PIN)){
    display.clearDisplay();
    display.display();
    // モーターに適用
    // motorP();  
    // motorRaw();
    Serial.println("move");    
  }else{
    static auto begin_ms = millis();
    buttonRead();
    for(auto b:button) Serial.print(b);
    if(buttonUp(0)) display_mode = (display_mode+1)%DISPLAY_MODE_NUM;

    clearVariables();
    addVariables("process", millis()-begin_ms);
    addVariables("ball_hold",millis()-ball_hold_begin);
    display.clearDisplay();
    debugDisplay(display_mode);
    display.display();
    begin_ms = millis();
    delay(25);
  }
}
