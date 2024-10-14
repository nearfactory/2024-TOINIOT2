// new atacker main program
// Teensy4.1
// 2024-09-11

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

  DISPLAY_MODE = DISPLAY_MODE::DIR;
  // ディスプレイモードを方向センサ用に変更
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

  delay(1000);
  // 攻め方向を取得・更新
  sensors_event_t d{};
  bno.getEvent(&d, Adafruit_BNO055::VECTOR_EULER);
  default_dir = d.orientation.x;
  
}

void loop() {

  static auto begin_ms = millis();
  digitalWrite(LED_BUILTIN, HIGH);
  if(use_display){
    display.clearDisplay();
    clearVariables();
  }

  // データを更新
  ballUpdate(BALL::DIR);
  buttonUpdate();
  dirUpdate();
  lineUpdate();
  // subUpdate();
  
  // 停止機能(ボタン3)
  // if(!use_display){
  // }
  if(buttonUp(3)){
    previous_button[2] = 0;
    printd(64, 32, "waiting...", TEXT_ALIGN::CENTER, TEXT_ALIGN::MIDDLE);
    display.display();
    // 再度ボタンを押すと再開
    motor_p_step = 16;
    while(!buttonUp(3)){
      buttonUpdate();
      motorSet(0.0f,0.0f,0.0f,0.0f);
      motorP();
      motorRaw();
      delay(40);
    }
    motor_p_step = motor_p_step_default;
    display.clearDisplay();
    display.display();
  }
  
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
  moveDir(move_dir, 100, true, 100);


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
