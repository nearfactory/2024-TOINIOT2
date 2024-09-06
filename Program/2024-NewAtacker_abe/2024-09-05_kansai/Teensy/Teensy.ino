// new atacker main program
// Teensy4.1
// 2024-07-30

#include <Wire.h>

#include "Ball.hpp"
#include "Communication.hpp"
#include "Dir.hpp"
#include "Kicker.hpp"
#include "Motor.hpp"
#include "UI.hpp"

using namespace std;

void setup() {
  // 通信/ボールセンサ/方向センサ/キッカー/モーター/UIのセットアップ
  communicationSetup();
  ballSetup();
  dirSetup();
  kickerSetup();
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

  // ディスプレイモードを方向センサ用に変更
  DISPLAY_MODE = DISPLAY_MODE::DIR;
  // キャリブレーションの状況変数を初期化 
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  // キャリブレーション完了まで繰り返し
  while(system!=3 || gyro!=3 || mag!=3){
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
  printd(120,56,"start",TEXT_ALIGN::RIGHT, TEXT_ALIGN::BOTTOM);
  display.display();

  // 3番ボタンが押された → ディスプレイなし
  // 4番ボタンが押された → ディスプレイあり
  // HIGH→LOWになったらループ脱出
  while(!buttonUp(3) && !buttonUp(4)){
    buttonUpdate();
    if(button[2]) use_display = false;
    if(button[3]) use_display = true;
  }

  display.clearDisplay();
  display.display();

  
  // 攻め方向を取得・更新
  dirUpdate();
  default_dir = dir;
  dir_default_display = -dir;
  
}

void loop() {

  static auto begin_ms = millis();
  if(use_display){
    digitalWrite(LED_BUILTIN, HIGH);
    display.clearDisplay();
    clearVariables();
  }

  // データを更新
  ballUpdate(BALL::DIR);
  buttonUpdate();
  dirUpdate();
  // subUpdate();
  
  // ディスプレイ不使用時
  if(!use_display){
    if(buttonUp(3)){
      previous_button[2] = 0;
      printd(64, 32, "waiting...", TEXT_ALIGN::CENTER, TEXT_ALIGN::MIDDLE);
      display.display();
      // 再度ボタンを押すと再開
      while(!buttonUp(3)){
        buttonUpdate();
        motorRaw(0.0f,0.0f,0.0f,0.0f);
        motorP();
        motorRaw();
        delay(40);
      }
      display.clearDisplay();
      display.display();
    }
  }
  
  // 回り込み


  // ボールを保持している
  if(ball_holding) {
  }

  // 白線避け


  // ボールが存在しない
  if(!ball_exist) motorSet(0.0f,0.0f,0.0f,0.0f);
  
  // 姿勢制御
  // setDir(dir,default_dir,60,40);
  
  // モーターに適用
  motorP();
  motorRaw();


  // UI (display)
  if(use_display){
    addVariables("process", millis()-begin_ms);
    addVariables("ball_hold",millis()-ball_hold_begin);
    begin_ms = millis();
    if(buttonUp(4)) DISPLAY_MODE = (DISPLAY_MODE+1)%DISPLAY_MODE_NUM;
    debugDisplay(DISPLAY_MODE);
    display.display();
  }
  
}
