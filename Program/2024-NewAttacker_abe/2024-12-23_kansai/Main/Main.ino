// new atacker kansai block program
// Teensy4.1
// 2024-12-24

#include <Wire.h>

#include "Ball.hpp"
#include "Camera.hpp"
#include "Dir.hpp"
#include "Display.hpp"
#include "Kicker.hpp"
#include "Line.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

Ball ball;
Camera camera;
Dir dir;
Display display;
Kicker kicker;
Line line;
Motor motor;
Sub sub;
UI ui;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai");

  ball.begin();
  camera.begin();
  display.begin();
  dir.begin();
  kicker.begin();
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


void loop() {
  // delayなしで3(ms)

  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();


  if(digitalRead(ui.TOGGLE_PIN)){
    if(ui.buttonUp(0)) display.next();
    // if(display.mode != MODE::VARIABLES){
    //   if(ui.buttonUp(0)) display.next();
    // }

    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    display.addValiables("d_gain :"+to_string(d_gain), &d_gain);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    is_display_on = true;

  }else{
    // ディスプレイを消灯
    if(is_display_on){
      display.draw();
      is_display_on = false;
    }




    bool keep_dir = false;
    // シュート
    if(ball.not_hold_time < 100){
      motor.moveDirFast(-camera.goal_dir*1.5, 100);

      if(ball.hold_time > 50 && abs(camera.prev_goal_dir) > 15.0 && camera.atk_h > 25){
        motor.moveDirFast(0, 100);

        float dir_power = camera.goal_dir * 8.0;
        motor.add(dir_power, dir_power, dir_power, dir_power);

      }
      
      if(100 <= camera.atk_w && camera.atk_w <= 150 && ball.hold_time > 250) kicker.kick();
    }
    // 回り込み(方法4) https://yuta.techblog.jp/archives/40889399.html
    else{
      float move_dir = 0;

      // PD
      if(abs(ball.dir)<h){
        move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
        keep_dir = true;
        h = 45;
      }
      // 円周上
      else if(ball.distance < r){
        float theta = 90 + (r-ball.distance) * 90 / r;
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        // keep_dir = true;
        h = 20;
      }
      //接線
      else{
        float theta = degrees(atan2(r, ball.distance));
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 20;
      }

      motor.moveDirFast(move_dir, 100);
    }



    // ボールが見えない場合に後ろに下がる (デバッグ段階では手前に)
    if(!ball.is_exist){
      motor.moveDir(180, 60);
    }



    // ストール
    // if(motor.raw_sum > 200 && dir.accel_sum < 1.0){
    //   float power = 80.0;
    //   motor.set(power, power, power, power);
    // }

    

    // 白線避け
    if(line.on){
      motor.moveDir(line.dir+180, 100);
    }



    // 姿勢制御
    // if(keep_dir){
    if(false){
      float d_gain = 0.45f;
      float p_gain = 0.64f;
      float dir_power = (dir.dir - keep_dir_goal) * p_gain + (dir.dir - dir.prev_dir) * d_gain;
      motor.add(dir_power, dir_power, dir_power, dir_power);

    }else{
      float d_gain = 0.45f;
      float p_gain = 0.64f;
      float dir_power = 0;

      if(abs(dir.dir) < 90){
        dir_power = (camera.goal_dir) * p_gain + (dir.dir - dir.prev_dir) * d_gain;
      }else{
        dir_power = (dir.dir) * p_gain + (dir.dir - dir.prev_dir) * d_gain;
      }

      if(abs(dir.dir) > 90) {
        // 故障復帰
        motor.set(dir_power, dir_power, dir_power, dir_power);
      }else{
        // 姿勢制御
        motor.add(dir_power, dir_power, dir_power, dir_power);
      }

      keep_dir_goal = dir.dir;
    }

    if(keep_dir){
      ui.buzzer(880.0f);
    }else{
      ui.buzzer(440.0f);
    }

  }


  motor.avr();
  motor.write();
  
  kicker.write();
}