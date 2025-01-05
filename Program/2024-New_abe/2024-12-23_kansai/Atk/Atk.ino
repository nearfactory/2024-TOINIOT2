// new atacker kansai block program
// Teensy4.1
// 2024-12-24

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
#include "Dir.hpp"
#include "Display.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Line line;
Motor motor;
Sub sub;
UI ui;

// #include "Kicker.hpp"
// Kicker kicker;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai");

  ball.begin();
  camera.begin();
  display.begin();
  dir.begin();
  line.begin(115200);
  motor.begin();
  sub.begin();
  ui.begin();
  
  // calibration
  uint8_t system=0, gyro=0, accel=0, mag=0;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);

    display.printd(32,16,"system: "+to_string(system));
    display.printd(32,24,"gyro  : "+to_string(gyro));
    display.printd(32,32,"accel : "+to_string(accel));
    display.printd(32,40,"mag   : "+to_string(mag));

    display.printd(120, 8, "skip", ALIGN::RIGHT);
    ui.read();
    if(ui.buttonUp(1)) break;

    display.draw();
  }

  // 攻め方向が設定されるまで待機
  display.printd(8,56,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  display.draw();
  
  dir.setDefault();
}


bool is_display_on = true;

// 周り込み
float h = 45;       // ヒステリシス
float r = 14400.0; // 回り込みの半径
float p_gain = 1.5;
float d_gain = 4.2;

// ボールの運搬
bool      shoot = false;
uint32_t  shoot_timer = 0;
float     shoot_dir_begin = 0;

// ボールのキック動作
bool     kick = false;
uint32_t kick_timer = 0;

float keep_dir_goal = 0;


uint32_t state_begin = 0;


void loop() {
  // delayなしで約3(ms)

  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();



  // ディスプレイ
  if(ui.is_toggle){
    Serial.println("display");
    if(ui.buttonUp(0)) display.next();

    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    display.addValiables("d_gain :"+to_string(d_gain), &d_gain);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    is_display_on = true;

    motor.avr();
    motor.write();
    delay(10);

    return;
  }
  // ディスプレイを消灯
  if(is_display_on){
    display.draw();
    is_display_on = false;
  }



  uint32_t state_elapsed = millis() - state_begin;




  // キックオフ
  if(state == State::KickOff){
    motor.moveDirFast(ball.dir, 100);


    // ボールを保持 -> ゴールに向かう
    if(ball.is_hold){
      state = State::Dribble;
    }

    // 1秒経過 -> 回り込み
    if(state_elapsed > 1000){
      state = State::Follow;
    }

  }



  // 回り込み
  else if(state == State::Follow){
    float move_dir = 0;

    // PD
    if(abs(ball.dir)<h){
      move_dir = ball.dir * p_gain - d_gain*(ball.dir -     ball.dir_prev);
      h = 45;
    }
    // 円周上
    else if(ball.distance < r){
      float theta = 90 + (r-ball.distance) * 90 / r;
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 20;
    }
    //接線
    else{
      float theta = degrees(atan2(r, ball.distance));
      move_dir = ball.dir + (ball.dir>0?theta:-theta);
      h = 20;
    }

    motor.moveDir(move_dir, 100);
    motor.setDir(dir.dir, dir.dir_prev, dir.p_gain, dir.d_gain);

   
    // ボールを保持 -> ゴールに向かう
    if(ball.is_hold){
      state = State::Dribble;
    }

  }



  // ゴールに向かう
  else if(state == State::Dribble){
    // キーパーのいない方のゴールの角を狙う
    


  }



  // キーパーをどかす
  else if(state == State::AvoidKeeper){
    // ゴール左側に向かいながら右を狙う
    
  }



  // 押し合い
  else if(state == State::Pushing){
    
  }



  // ボールが取り上げられた
  else if(state == State::NoBall){
    // 近くの中立点の前まで移動する
    
  }



  // 中立点からの回り込み・シュート
  else if(state == State::Neutral){
    // ゴールの方に45度傾ける
    
  }
  else{
    state = State::Follow;
  }


  motor.avr();
  motor.write();
}