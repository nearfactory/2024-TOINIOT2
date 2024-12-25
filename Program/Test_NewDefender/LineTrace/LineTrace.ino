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
  Serial.println("2024-12-24 TOINIOT2 kansai def");

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


float move_p_gain = 0;
float move_d_gain = 0;


void loop() {
  // delayなしで3(ms)

  ball.read();
  camera.read();
  dir.read();
  line.read();
  sub.read();
  ui.read();


  if(digitalRead(ui.TOGGLE_PIN)){
    // if(ui.buttonUp(0)) display.next();

    display.addValiables("p_gain :"+to_string(move_p_gain), &move_p_gain);
    display.addValiables("d_gain :"+to_string(move_d_gain), &move_d_gain);

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


    float move_dir = 0;
    if(ball.dir > 0)  move_dir = 90.0;
    else              move_dir = -90.0;

    float move_p = abs(sin(radians(ball.dir)))*100.0;
    float move_d = abs(sin(radians(ball.dir)))*100.0 - abs(sin(radians(ball.dir_prev)))*100.0;

    int power = move_p * move_p_gain + move_d * move_d_gain;
    motor.moveDir(move_dir, power);



    // 姿勢制御

    float p_gain = 0.64f;
    float d_gain = 0.45f;
    float dir_power = (dir.dir) * p_gain - (dir.prev_dir - dir.dir) * d_gain;

    if(abs(dir.dir) > 45) {
      // 故障復帰
      motor.set(dir_power, dir_power, dir_power, dir_power);
    }else{
      // 姿勢制御
      motor.add(dir_power, dir_power, dir_power, dir_power);
    }

  }


  motor.avr();
  motor.write();
  
  kicker.write();
}