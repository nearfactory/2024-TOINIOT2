// new atacker main program
// Teensy4.1
// 2024-09-11

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
  Serial.println("begin");
  Serial7.begin(115200);

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
    display.draw();

    ui.read();
    if(ui.buttonUp(1)) break;

  }

  // 攻め方向が設定されるまで待機
  digitalWrite(LED_BUILTIN, LOW);
  display.printd(8,56,"set dir");
  display.draw();
  while(!ui.buttonUp(0)){ ui.read(); }
  
  dir.setDefault();
}


bool is_display_on = true;

// 回り込み
int h = 45; // ヒステリシス
float r = 14400.0f;  // 回り込み時の半径
float p_gain = 1.5f;
float d_gain = 3.5f;
float* gain_select = &p_gain;

// ボールの運搬
bool      shoot = false;
uint32_t  shoot_timer = 0;
float     shoot_dir_begin = 0;

// ボールのキック動作
bool     kick = false;
uint32_t kick_timer = 0;

// 白線に阻まれた場合、ボールに直進
int      line_count = 0;  // 回り込み中に白線を踏んだ回数
uint32_t line_begin = 0;  // 白線を踏み始めた時間
uint32_t straight_begin = 0;

void loop() {
  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();

  bool face_to_ball = false;

  if(digitalRead(ui.TOGGLE_PIN)){
    // 25(ms)
    ui.read();
    if(ui.buttonUp(0) && display.mode != MODE::VARIABLES) display.next();

    display.addValiables("p_gain :"+to_string(p_gain), &p_gain);
    display.addValiables("d_gain :"+to_string(d_gain), &d_gain);

    display.debug();
    display.draw();

    motor.set(0,0,0,0);
    is_display_on = true;

  }else{
    // ディスプレイを消灯
    if(is_display_on){
      // ボタンが足りないため、変数の表示時のみトグルスイッチをモード切替用に使用する
      if(display.mode == MODE::VARIABLES){
        display.next();
      }
      display.draw();
      is_display_on = false;
    }




    // シュート
    if(shoot){
      // ゴールに向かって移動
      // float shoot_dir = 0;
      // if(shoot_dir_begin < -15){
      //   shoot_dir = 15;
      // }else if(shoot_dir_begin < 15){
      //   shoot_dir = 0;
      // }else{
      //   shoot_dir = -15;
      // }
      // motor.moveDirFast(shoot_dir, 100);
      motor.moveDirFast(-camera.goal_dir* 1.2, 100);
      
      if(abs(camera.goal_dir) > 30){
        float dir_power = camera.goal_dir;
        motor.add(dir_power, dir_power, dir_power, dir_power);
      }

      // シュート終了
      if(!ball.is_hold) shoot = false;

      // ゴール前がガラ空きの場合にボールをキックする
      if(camera.atk_num == 1 && camera.atk_w > 100){
        kick = true;
        kick_timer = millis();
      }

      // キック動作
      if(kick){
        // ゴールに向ける
        if(50 < millis() - kick_timer && millis() - kick_timer < 150){
          float dir_power = camera.goal_dir * 6.0;
          motor.add(dir_power, dir_power, dir_power, dir_power);
        }
        // キック
        if(100 < millis() - kick_timer){
          kicker.kick();
        }

        // キック終了
        if(150 < millis() - kick_timer){
          shoot = false;
          kick = false;
        }
      }
        

    }
    // 回り込み(方法4) https://yuta.techblog.jp/archives/40889399.html
    else{
      float theta = 0;
      float move_dir = 0;
      // PD
      if(abs(ball.dir)<h){
        move_dir = ball.dir * p_gain - d_gain*(ball.dir - ball.dir_prev);
        h = 45;

        // シュート動作に入る
        if(ball.is_hold){
          shoot = true;
          shoot_dir_begin = camera.goal_dir;
          shoot_timer = millis();
        } 
      }
      // 円周上
      else if(ball.distance < r){
        theta = 90 + (r-ball.distance) * 90 / r;
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 45;
      }
      //接線
      else{
        theta = degrees(atan2(r, ball.distance));
        move_dir = ball.dir + (ball.dir>0?theta:-theta);
        h = 45;
      }
      motor.moveDir(move_dir, 100);

      // 白線に繰り返し触れた場合、ボールに向かって直進する
      if(line.prev_on == false && line.on == true && millis()-line_begin < 400){
        line_begin = millis();
        line_count++;
      }
      // 3回以上触れた場合、
      if(line_count >= 3){
        line_count = 0;
        line_begin = 0;
        straight_begin = millis();
      }
      // 直進
      if(millis() - straight_begin > 800){
        motor.moveDir(ball.dir, 100);
      }

    }




    // ボールが見えない場合に後ろに下がる (デバッグ段階では手前に)
    if(!ball.is_exist){
      motor.moveDir(180, 60);
    }

    


    // 白線避け
    if(line.on){
      motor.moveDir(line.dir+180, 100);
    }

    // 外側の白線避け(力技)
    if(line.outside){
      static uint32_t front_timer = 0;
      static uint32_t left_timer  = 0;
      static uint32_t back_timer  = 0;
      static uint32_t right_timer = 0;

      if(line.front) front_timer = millis();
      if(line.left)  left_timer  = millis();
      if(line.back)  back_timer  = millis();
      if(line.right) right_timer = millis();

      Vec2 avoid_vec(0, 0);

      if(millis()-front_timer < 160) avoid_vec.y = -1.0;
      if(millis()-left_timer  < 160) avoid_vec.x =  1.0;
      if(millis()-back_timer  < 160) avoid_vec.y =  1.0;
      if(millis()-right_timer < 160) avoid_vec.x = -1.0;

      float avoid_dir = degrees(atan2(avoid_vec.y, avoid_vec.x));
      motor.moveDir(avoid_dir, 100);

    }




    // 姿勢制御
    float p_gain = 0.64f;
    float d_gain = 0.45f;

    float dir_power = dir.dir * p_gain - (dir.prev_dir - dir.dir) * d_gain;

    // 反時計回りの場合に少し弱める
    if(dir_power > 0) dir_power *= 0.9;

    if(abs(dir.dir) > 45) {
      // 故障復帰用
      motor.set(dir_power, dir_power, dir_power, dir_power);
    }else{
      motor.addRaw(dir_power, dir_power, dir_power, dir_power);
    }

  }

  // ui
  if(line.outside){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }

  motor.avr();
  motor.write();
  
  kicker.write();

  // delayなしで3(ms)

  /*
  ToDo:
    1. コートの四隅での白線避け
        左右の白線避けに使っているアルゴリズムを前後にも適用、同時に反応した場合にも対応できるようにする
    2. 回り込み中に捕獲エリアの角に当てないようにする
        角度と倍率・PD制御のゲイン調整
    3. ボールを保持したまま、ゴールの方向に移動できるようにする
        ボールを捕獲した瞬間のゴールの角度で条件分岐を行う
    4. 両側のゴールに向かえるように書き換える
        sigをどちらのゴールに対しても設定できるように
    4. 敵がいない場合にキッカーを作動させる
        pixyのブロック数、面積で条件分岐
    5. キッカーの作動直前にゴールに向ける
        タイミングの調整
    6. 回り込み中にラインに触れ続けた場合に、ボールに直線的に移動する
  */

}