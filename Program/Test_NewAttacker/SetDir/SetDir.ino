#include <string>

#include <Arduino.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Motor.hpp"

using namespace std;

Adafruit_BNO055 bno(55, 0x28, &Wire2);
double default_dir = 0;


constexpr uint8_t TOGGLE_PIN = 36;


constexpr uint8_t BUTTON_NUM = 4;
const uint8_t BUTTON_PIN[BUTTON_NUM] = { 37,31,30,32 };
bool    button[BUTTON_NUM] = {false};
bool    previous_button[BUTTON_NUM] = {false};

inline void buttonRead(){
  for(int i=0;i<BUTTON_NUM;i++){
    previous_button[i] = button[i];
    button[i] = digitalRead(BUTTON_PIN[i]);
  }
  return;
}

inline bool buttonUp(uint8_t num){
  num = num<0 ? 0 : num;
  num = num>3 ? 3 : num;
  return (!button[num]) && previous_button[num];
}


Adafruit_SSD1306 display(128, 64, &Wire2, -1);
enum class TEXT_ALIGN : uint8_t{
  LEFT = 0,
  CENTER,
  RIGHT,
  
  TOP,
  MIDDLE,
  BOTTOM
};

inline void printd(uint8_t x, uint8_t y, std::string str, TEXT_ALIGN align_x = TEXT_ALIGN::LEFT, TEXT_ALIGN align_y = TEXT_ALIGN::TOP){
  switch(align_x){
    case TEXT_ALIGN::LEFT :
      break;
    case TEXT_ALIGN::CENTER :
      x -= str.length()*8/2;
      break;
    case TEXT_ALIGN::RIGHT :
      x -= str.length()*8;
      break;
  }
  switch(align_y){
    case TEXT_ALIGN::TOP :
      break;
    case TEXT_ALIGN::MIDDLE :
      y -= 8/2;
      break;
    case TEXT_ALIGN::BOTTOM :
      y -=8;
      break;
  }
  display.setCursor(x, y);
  display.print(str.c_str());

  return;
}


void setup(){
  Serial.begin(9600);
  Serial.println("TOINIOT2 SetDir Test 2024-09-04");

  Wire2.begin();

  if(!bno.begin()){
    Serial.println("bno err!");
    while(true);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("display err!");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
  // system = 3; gyro = 3; mag = 3;
  // キャリブレーション完了まで繰り返し
  while(system<3 || gyro<3 || mag<3){
    display.clearDisplay();
    display.setCursor(8,8);
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

  Serial.println("setup()");

  while(!buttonUp(0)){
    buttonRead();
    delay(50);
  }

  delay(500);
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  default_dir = orientation.orientation.x;
}

static double p_gain = 0.67;
static double i_gain = 0.0;
static double d_gain = 0.45;

static double p = 0;
static double i = 0;
static double d = 0;

double* gain_ptr = &p_gain;
double gain_unit = 0.1;

void loop(){
  sensors_event_t orientation;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  double dir = orientation.orientation.x - default_dir;
  static double prev_dir = dir;

  if(dir>180)  dir = dir-360;
  if(dir<-180) dir = dir+360;

  // モーター出力を計算

  p = dir;
  i += dir/10000;
  d = prev_dir - dir;

  if(i > 100.0) i = 100.0;

  double power = p*p_gain + i*i_gain - d*d_gain;

  prev_dir = dir;

  buttonRead();
  if(digitalRead(TOGGLE_PIN)){
    if(buttonUp(3)){
      if(gain_ptr == &p_gain)      gain_ptr = &i_gain;
      else if(gain_ptr == &i_gain) gain_ptr = &d_gain;
      else if(gain_ptr == &d_gain) gain_ptr = &p_gain;
    }
    else if(buttonUp(1)) *gain_ptr += gain_unit;
    else if(buttonUp(2)) *gain_ptr -= gain_unit;
    else if(buttonUp(0)){
      if(gain_unit == 1)         gain_unit = 0.1;
      else if(gain_unit == 0.1)  gain_unit = 0.01;
      else if(gain_unit == 0.01) gain_unit = 1.0;
    }

    display.clearDisplay();
    Serial.printf("dir:%d power:%d\n", dir, power);
    printd(0,128,"u:"+to_string(gain_unit), TEXT_ALIGN::RIGHT, TEXT_ALIGN::TOP);

    if(gain_ptr == &p_gain) display.setCursor(64,32);
    if(gain_ptr == &i_gain) display.setCursor(64,40);
    if(gain_ptr == &d_gain) display.setCursor(64,48);
    display.print("<");

    display.setCursor(0, 0);
    // display.println("val:");
    display.printf("p:%f\ni:%f\nd:%f\n", p, i, d);
    display.println("gain:");
    display.printf("p:%f\ni:%f\nd:%f\n", p_gain, i_gain, d_gain);
    display.printf("pow:%f", power);
    display.display();

    motor_raw[0] = power;
    motor_raw[1] = power;
    motor_raw[2] = power;
    motor_raw[3] = power;
    motorRaw();
  }else{
    motor[0] = 0;
    motor[1] = 0;
    motor[2] = 0;
    motor[3] = 0;
    motorP();
    motorRaw();

    display.clearDisplay();
    display.display();
  }

  delay(50);
}