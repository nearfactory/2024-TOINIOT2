// new atacker ritsumori
// Teensy4.1
// 2024-02-23

#include <cmath>

#include <Wire.h>

#include "Display.hpp"
#include "Motor.hpp"
#include "Sub.hpp"
#include "UI.hpp"

Display display;
Motor motor;
Sub sub;
UI ui;


using namespace std;


bool is_display_on = true;

float offset = 1.0;



void setup() {
  Serial.begin(115200);
  Serial.println("2024-12-24 TOINIOT2 kansai");

  display.begin();
  motor.begin();
  sub.begin();
  ui.begin();

  Serial.println("start");
}



void loop() {
  sub.read();
  ui.read();

  // ディスプレイ
  if(ui.is_toggle){
    Serial.println("display");

    if(ui.buttonUp(0)) display.next();

    display.debug();
    display.draw();
    is_display_on = true;

    motor.set(0,0,0,0);

    motor.avr();
    motor.write();
    delay(4);

    return;
  }
  // ディスプレイを消灯
  if(is_display_on){
    display.draw();
    is_display_on = false;
  }


  

  // main
  if(sub.str == "k"){
    sub.kick();
  }else{
    motor.moveDir(0, 40);
  }




  motor.avr();
  motor.write();
}