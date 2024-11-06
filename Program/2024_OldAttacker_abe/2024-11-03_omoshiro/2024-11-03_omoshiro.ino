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
  Serial.println("TOINTIOT2 kansai old atk");
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
  // system = 3;
  // mag = 3;
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

    // bzUpdate(440.0f);
    LEDUpdate(1,1,1);

    delay(10);
  }
  bzUpdate(0.0f);

  // スタート画面を表示

  // 攻め方向を取得・更新
  // HIGH→LOWになったらループ脱出
  while(!buttonUp(0)){
    LEDUpdate(1,0,0);
    buttonUpdate();
  }
  sensors_event_t d{};
  bno.getEvent(&d, Adafruit_BNO055::VECTOR_EULER);
  default_dir = d.orientation.x;

  // 開始
  while(!buttonUp(1)){
    LEDUpdate(0,1,0);
    buttonUpdate();
  }

  LEDUpdate(0,0,0);
}

double goal_dir = 0;
double atk_dir = 0;

void loop() {
  buttonUpdate();
  if(!button[3]){
    // line
    short power = 100;
    lineUpdate();
    if(line[0] || line[1] || line[2] || line[3]){
      motorSet(-power,-power,power,power);
      for(int i=0;i<5;i++){
        motorP();
        motorRaw();
      }
      delay(300);
      goto SKIP_ALL;
    }else if(line[4] || line[5] || line[6] || line[7] || line[8] || line[9]){
      motorSet(power,-power,-power,power);
      goal_dir = 25;
      goto SKIP_ALL;
    }else if(line[10] || line[11] || line[12] || line[13] || line[14] || line[15]){
      motorSet(power,power,-power,-power);
      goto SKIP_ALL;
    }else if(line[16] || line[17] || line[18] || line[19] || line[20] || line[21]){
      motorSet(-power,power,power,-power);
      goal_dir = -25;
      goto SKIP_ALL;
    }
    // Serial.print(" ");
    // for(int i=0;i<LINE_NUM;i++){
    //   if(i==4 || i==10 || i==16) Serial.print(" ");
    //   Serial.print(line[i]);
    // } 

    // ball
    ballUpdate(BALL::DIR);
    // Serial.print("min:-180 max:180 dir:");
    // Serial.print(ball_dir);
    // Serial.print(" distance:");
    // Serial.print(ball_distance);

    atk_dir = 0;

    power = 80;
    short power2 = 90;
    switch(ball_small_id){
      case 0:
        // Serial.println("0");
        if(ball_distance < 8500){
          atk_dir = goal_dir;
        }
        motorSet(power2,power2,-power2,-power2);
        break;
      case 1:
        // Serial.println("1");
        motorSet(0,power,0,-power);
        break;
      case 2:
      case 3:
        // Serial.println("23");
        motorSet(-power2,power2,power2,-power2);
        break;
      case 4:      
      case 5:
        // Serial.println("45");
        motorSet(-power,0,power,0);
        break;
      case 6:
      case 7:
      case 9:
      case 10:
        // Serial.println("67910");
        motorSet(-power2,-power2,power2,power2);
        break;
      case 8:
        // Serial.println("8");
        motorSet(-power2,power2,power2,-power2);
        break;
      // back
      case 11:
      case 12:
        // Serial.println("1112");
        motorSet(0,-power,0,power);
        break;
      case 13:
      case 14:
        // Serial.println("1314");
        motorSet(power2,-power2,-power2,power2);
        break;
      case 15:
        // Serial.println("15");
        motorSet(power,0,-power,0);
        break;
    }

    // dir
    dirUpdate();
    // double DIR_TORELANCE = 15.0f;
    // if(dir < -DIR_TORELANCE || DIR_TORELANCE < dir){
    //   setDir(dir,0,70,100);
    // }else{
    //   setDir(dir,0,60,40);
    // }
    setDir(dir, atk_dir, 60, 40);

    if(!ball_exist) motorSet(0.0f, 0.0f, 0.0f, 0.0f);

    bzUpdate(0.0f);
    // Serial.println();
    Serial.println(dir);

SKIP_ALL:
    motorP();
    motorRaw();
    // Serial.println(dir);
    // delay(50);
  }else{
    // Serial.println("off");
    bzUpdate(440.0f);
    LEDUpdate(1,1,1);

    motorSet(0, 0, 0, 0);
    motorP();
    motorRaw();
    delay(50);
  }

    
  //   // follow ball
  //   if(ball_exist){
  //     // Serial.println("ball");
  //     // Serial.println("ball");
  //     // Serial.print("distance:");
  //     // Serial.println(ball_distance);

  //     short power = 100;
  //     short power2 = power / 1.4;
  //     switch(ball_small_id){
  //       case 0:
  //         // Serial.println("0");
  //         motorSet(power2,power2,-power2,-power2);
  //         break;
  //       case 1:
  //         // Serial.println("1");
  //         motorSet(0,power,0,-power);
  //         break;
  //       case 2:
  //       case 3:
  //         // Serial.println("23");
  //         motorSet(-power2,power2,power2,-power2);
  //         break;
  //       case 4:
  //         // Serial.println("45");
  //         motorSet(-power,0,power,0);
  //         break;
  //       case 5:
  //       case 6:
  //       case 7:

  //       case 9:
  //       case 10:
  //       case 11:
  //         // Serial.println("67910");
  //         motorSet(-power2,-power2,power2,power2);
  //         break;
  //       case 8:
  //         // Serial.println("8");
  //         motorSet(-power2,power2,power2,-power2);
  //         break;
  //       // back
  //       case 12:
  //         // Serial.println("1112");
  //         motorSet(0,-power,0,power);
  //         break;
  //       case 13:
  //       case 14:
  //         // Serial.println("1314");
  //         motorSet(power2,-power2,-power2,power2);
  //         break;
  //       case 15:
  //         // Serial.println("15");
  //         motorSet(power,0,-power,0);
  //         break;
  //     }

}
