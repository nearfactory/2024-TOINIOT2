// new atacker main program
// Teensy4.1
// 2024-09-11

#include "Ball.hpp"
#include "Dir.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

int mode = 0;

using namespace std;

void setup() {
  Serial.begin(9600);
  // 通信/ボールセンサ/方向センサ/キッカー/モーター/UIのセットアップ
  ballSetup();
  dirSetup();
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

  // ディスプレイモードを方向センサ用に変更
  // キャリブレーションのu状況変数を初期化 
  
  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  system = 3;
  mag = 3;
  // キャリブレーション完了まで繰り返し
  // while(system!=3 || gyro!=3 || mag!=3){
  while(system!=3 || mag!=3){
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print("sytem:");
    Serial.print(system);
    // Serial.print("gyro:");
    // Serial.print(gyro);
    Serial.print("mag:");
    Serial.println(mag);

    bzUpdate(440.0f);

    delay(10);
  }
  bzUpdate(0.0f);

  // スタート画面を表示

  // Aボタン → 新プログラム
  // Bボタン → 旧プログラム
  // HIGH→LOWになったらループ脱出
  while(!buttonUp(0) && !buttonUp(1)){
    if(buttonUp(0)) mode = 0;
    if(buttonUp(1)) mode = 1;
    buttonUpdate();
  }

  delay(1000);
  // 攻め方向を取得・更新
  sensors_event_t d{};
  bno.getEvent(&d, Adafruit_BNO055::VECTOR_EULER);
  default_dir = d.orientation.x;
  
}

void loop() {
  if(mode == 0){   
    if(buttonUp(2)){
      previous_button[2] = 0;
      // 再度ボタンを押すと再開
      motor_p_step = 8;
      while(!buttonUp(2)){
        buttonUpdate();
        motorSet(0.0f,0.0f,0.0f,0.0f);
        motorP();
        motorRaw();
        delay(25);
      }
      motor_p_step = motor_p_step_default;
    }

    static auto begin_ms = millis();

    // データを更新
    ballUpdate(BALL::DIR);
    buttonUpdate();
    dirUpdate();
    lineUpdate();
    
    // Serial.println(ball_dir);
    
    LEDUpdate();

    // moveDir(0,40,true,100);
    Serial.println(ball_small_id);
    int power = 40;
    switch(ball_small_id){
      case 0:
      case 1:
      case 15:
        // moveDir(0,power,true,100);
        moveDir(0,2,true);
        break;
      case 2:
        moveDir(-45,power,true);
        // -135
        break;
      case 3:
        moveDir(-90,power,true);
        // 0
        break;
      case 4:
        moveDir(-135,power,true);
        // -45
        break;
      case 5:
      case 6:
        moveDir(-180,power,true);
        // 0
        break;
      case 7:
      case 8:
      case 9:
        moveDir(-90,power,true);
        // -90
        break;
      case 10:
      case 11:
        moveDir(180,power,true);
        // 0
        break;
      case 12:
        moveDir(135,power,true);
        // 45
        break;
      case 13:
        moveDir(90,power,true);
        // 90
        break;
      case 14:
        moveDir(45,power,true);
        // 135
        break;
    }

    // 白線避け
    // avoidLine();

    if(!ball_exist) motorSet(0,0,0,0);

    double torelance = 5;
    // if(!(-torelance<dir && dir<torelance))
    // setDir(dir,0,100,20);

    // モーターに適用
    motorP();
    for(auto m:motor_raw){
      Serial.print(m);
      Serial.print(" ");
    }
    Serial.println();
    motorRaw();
    delay(25);
  }

}
