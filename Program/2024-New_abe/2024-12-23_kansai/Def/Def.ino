// old defender main program
// ATmega2560
// 2024-12-14

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
#include "Dir.hpp"
#include "Display.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Line line;
Motor motor;
UI ui;

using namespace std;



// ステートマシン
enum class State{
  LineTrace,
  KeeperDash,
  BackToGoal_Weak,
  BackToGoal_Strong
};
State state = State::LineTrace;



void setup() {
  Serial.begin(115200);
  Serial.println("begin");

  ball.begin();
  display.begin();
  dir.begin();
  line.begin();
  motor.begin();
  

  uint8_t system=0, gyro=0, accel=0, mag=0;
  // system = 3; gyro = 3; mag = 3;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    ui.buzzer(440.0f);

    char str = "";
    sprintf(str, "system: %d",system);
    display.printd(32,16,str);
    sprintf(str, "gyro  : %d",gyro);
    display.printd(32,24,str);
    sprintf(str, "accel : %d",accel);
    display.printd(32,32,str);
    sprintf(str, "mag   : %d",mag);
    display.printd(32,40,str);
    display.draw();
  }

  // 攻め方向が設定されるまで待機
  display.printd(8,8,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  
  dir.setDefault();
}


bool display_on = true;


void loop() {
  ball.read();
  dir.read();
  line.read();

  if(digitalRead(ui.TOGGLE)){
    ui.read();
    if(ui.buttonUp(0)) display.next();

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    display_on = true;

    motor.avr();
    motor.write();
    delay(10);

    return;
  }
  // ディスプレイを消灯
  if(display_on){
    display.draw();
    display_on = false;
  }



  // 1.ライントレース
  if(state == State::LineTrace){
    // ペナルティエリア内にボールがあるか
    bool is_ball_penalty_area = false;
    if(is_ball_penalty_area){

    }


    // トリガー
    static uint32_t ball_front_begin = 0;

    static bool is_ball_front = false;
    static bool is_ball_front_prev = false;


    is_ball_front_prev = is_ball_front;
    is_ball_front = ball.distance < 14400 && abs(ball.dir) < 22.5;


    if(is_ball_front == true && is_ball_front_prev == false){
      ball_front_begin = millis();
    }


    // →  2.キーパーダッシュ
    if(millis()-ball_front_begin > 2000){
      state = State::KeeperDash;
    }
    // →  3.ゴール前に戻る(弱め)
    else if(line.on == false){
      state = State::BackToGoal_Weak;
    }

  }



  // 2.キーパーダッシュ
  else if(state == State::KeeperDash){
    bool is_line = false;
    bool is_stop = false;

    // → 4.ゴール前に戻る(強め)
    if(is_line || is_stop){
      state = State::BackToGoal_Strong;
    }
  }
  


  // 3.ゴール前に戻る（弱め）
  else if(state == State::BackToGoal_Weak){
    // 最後のラインのベクトルへ移動
    motor.moveDir(line.dir, 100);


    // トリガー
    static uint32_t is_not_line_begin = 0;
    if(line.on_prev == true && line.on == false){
      is_not_line_begin = millis();
    }

    // →  1.ライントレース
    if(line.on){
      state = State::LineTrace;
    }
    // →  4.ゴール前に戻る(強め)
    else if(millis()-is_not_line_begin > 2000){
      state = State::BackToGoal_Strong;
    }
  }

  

  // 4.ゴール前に戻る（強め）
  else if(state == State::BackToGoal_Strong){
    motor.moveDir(camera.def_dir, 100);
    motor.setDirAdd(dir.dir);

    // →  1.ライントレース
    if(line.on){
      state = State::LineTrace;
    }
  }
  
  

  // 5.エラー
  else{
    state = State::LineTrace;
  }



  motor.avr();
  motor.write();

  delay(10);
}

















