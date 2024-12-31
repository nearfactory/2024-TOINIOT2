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
  printd(120,56,"start",TEXT_ALIGN::CENTER, TEXT_ALIGN::BOTTOM);
  display.display();

  // 4番ボタンが押されるまで待機
  // HIGH→LOWになったらループ脱出
  while(!buttonUp(4)){
    buttonUpdate();
  }

  
  // 攻め方向を取得・更新
  dirUpdate();
  default_dir = dir;
  dir_default_display = -dir;
  
}

void loop() {

  static auto begin_ms = millis();
  digitalWrite(LED_BUILTIN, HIGH);
  display.clearDisplay();

  // データを更新
  ballUpdate(BALL::DIR);
  buttonUpdate();
  dirUpdate();
  // subUpdate();

  clearVariables();

  static short motor_dir = 0;
  motor_dir += button[0];
  motor_dir -= button[1];

  
  // 回り込み
  short difference=0;
  if(abs(ball_dir)>15){
    difference = ball_dir<0?-70:70;
  }else{
    difference = 20*ball_dir/15;
    // difference = ball_dir<0?-difference:difference;
  }
  moveDir(ball_dir+difference,80,false);


  // ボールを保持している
  if(ball_holding) {
    moveDir(0.0, 90, true);
    if(millis()-kicked_ms>5000 && millis()-ball_hold_begin > 300){
      kick();
    }
  }

  // ボールが存在しない
  // if(!ball_exist) motorRaw(0,0,0,0);
  
  // 姿勢制御
  setDir(dir,default_dir,60,40);
  
  // モーターに適用
  motorP();
  // motorRaw();


  // UI (display)
  addVariables("process", millis()-begin_ms);
  addVariables("ball_hold",millis()-ball_hold_begin);
  begin_ms = millis();
  if(buttonUp(4)) DISPLAY_MODE = (DISPLAY_MODE+1)%DISPLAY_MODE_NUM;
  debugDisplay(DISPLAY_MODE);
  display.display();
  
}
