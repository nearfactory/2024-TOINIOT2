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
Dir dir;
Display display;
Line line;
Motor motor;
UI ui;

using namespace std;

void setup() {
  Serial.begin(115200);
  Serial.println("begin");

  ball.begin();
  display.begin();
  dir.begin();
  line.begin();
  motor.begin();
  
  /*
  */
  uint8_t system=0, gyro=0, accel=0, mag=0;
  // system = 3; gyro = 3; mag = 3;
  while(system<3 || gyro<3 || mag<3){
    dir.calibration(&system, &gyro, &accel, &mag);
    digitalWrite(LED_BUILTIN, HIGH);

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
  }else{
    // ディスプレイを消灯
    if(display_on){
      display.draw();
      display_on = false;
    }

    Serial.print(ball.dir);
    Serial.print("\t");
    Serial.print(line.front);
    Serial.print(line.left);
    Serial.print(line.back);
    Serial.print(line.right);

  }

  motor.avr();
  motor.write();

  delay(10);
}
