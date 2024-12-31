#include <string>
#include "HardwareSerial.h"
#include "Display.hpp"

using namespace std;

extern float r;

void Display::begin(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Display err!");
    while(true);
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);

  return;
}

void Display::clear(){
  display.clearDisplay();
  display.display();

  return;
}

void Display::draw(){
  display.display();
  display.clearDisplay();

  variables.clear();
  valiables_addr.clear();
    
  return;
}

void Display::next(){
  mode = (mode+1)%MODE_NUM;
  return;
}



void Display::drawAngleLine(uint8_t cx, uint8_t cy, float angle, uint8_t r){
  display.drawLine(cx, cy, cx+cos(radians(angle))*r, cy+sin(radians(angle))*r, WHITE);
  return;
}

void Display::printd(uint8_t x, uint8_t y, std::string str, ALIGN align_x, ALIGN align_y){
  switch(align_x){
    case ALIGN::LEFT :
      break;
    case ALIGN::CENTER :
      x -= str.length()*8/2;
      break;
    case ALIGN::RIGHT :
      x -= str.length()*8;
      break;
    default:
      break;
  }
  switch(align_y){
    case ALIGN::TOP :
      break;
    case ALIGN::MIDDLE :
      y -= 8/2;
      break;
    case ALIGN::BOTTOM :
      y -=8;
      break;
    default:
      break;
  }
  display.setCursor(x, y);
  display.print(str.c_str());

  return;
}



void Display::addValiables(std::string name, float* addr){
  variables.emplace_back(name);
  valiables_addr.emplace_back(addr);
  return;
}



void Display::debug(){
  debug(mode);
  return;
}

void Display::debug(uint8_t mode){
  switch(mode){
    case MODE::BALL :
      this->Ball();
      break;
    case MODE::BLE :
      this->Ble();
      break;
    case MODE::CAMERA :
      this->Camera();
      break;
    case MODE::DIR :
      this->Dir();
      break;
    case MODE::DRIBBLER :
      this->Dribbler();
      break;
    case MODE::KICKER :
      this->Kicker();
      break;
    case MODE::LINE :
      this->Line();
      break;
    case MODE::MOTOR :
      this->Motor();
      break;
    case MODE::VARIABLES :
      this->Valiables();
      break;
    case MODE::GAME :
      this->Game();
      break;
    default:
      for(size_t i=0;i<variables.size();i++){
        printd(8,24+i*8,variables[i]);
      }
      printd(8,8,"internal error!");
      break;
  }

  return;
}


void Display::Ball(){
  printd(8,8,"Ball");
  uint8_t circle_r = 24;

  string str = to_string(ball.dir);
  str.erase(str.begin()+4,str.end());
  printd(8, 40, str);

  printd(8,48,"exist:"+to_string(ball.is_exist));
  printd(8,56,"hold :"+to_string(ball.is_hold));
  
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, ball.dir+180, circle_r);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 0, 8);

  printd(80, 32,to_string(ball.hold_time));
  printd(80, 40,to_string(ball.not_hold_time));
  printd(120,56,to_string(ball.distance), ALIGN::RIGHT);
  for(int i=0;i<16;i++){
    double angle = (i*360/16+180);

    angle = angle/180*3.14;
    uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
    uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
    display.drawPixel(x, y, WHITE);
  }

  return;
}

void Display::Ble(){
  printd(8,8,"BLE");
  /*
  printd(8, 24, "ATK:"+BLE_atk);
  printd(8, 40, "DEF:"+BLE_def);
  */

  return;
}

void Display::Camera(){
  printd(8,8,"Camera");
  
  // 画像サイズを示す枠
  int width = 76;
  int height = 48;

  printd(8,16,to_string(camera.goal_dir));

  printd(104, 8,  to_string(camera.atk_x));
  printd(104, 16, to_string(camera.atk_y));
  printd(104, 24, to_string(camera.atk_w));
  printd(104, 32, to_string(camera.atk_h));

  display.drawRect(64-width/2, 16, width, height, WHITE);

  // 検出されたブロック全体のバウンディングボックス
  for(int i=0;i<camera.block_num;i++){
    display.fillRect(
      26+ camera.block[i].x *width/320.0,
      16+ camera.block[i].y *height/200.0,
      camera.block[i].width  *width/320.0,
      camera.block[i].height *height/200.0,
      WHITE
    );
  }
  return;
}

void Display::Dir(){
  printd(8,8,"Dir");

  uint8_t circle_r = 24;
  string str = to_string(dir.dir);
  str.erase(str.begin()+5,str.end());
  printd(8, 32, str);

  printd(8,16,to_string(dir.accel_sum));
  printd(8,24,to_string(motor.raw_sum));

  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 180, 24);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 180-dir.dir, 16);

  for(int i=0;i<8;i++){
    double angle = i*360/8 -dir.dir;
    angle = angle/180*3.14;
    uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
    uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
    display.drawPixel(x, y, WHITE);
  }
  
  return;
}

void Display::Dribbler(){
  printd(8,8,"Dribbler");
  printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);
  return;
}

void Display::Kicker(){
  printd(8,8,"Kicker");
  // printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);

  int width = 24;
  int height = 48;
  display.drawRect(8, 8, width, height, WHITE);
  display.drawRect(96, 8, width, height, WHITE);

  display.fillRect(8,  8+(255-sub.ball01k)*height/255, width, (sub.ball01k)*height/255, WHITE);
  display.fillRect(96, 8+(255-sub.ball02k)*height/255, width, (sub.ball02k)*height/255, WHITE);

  printd(120,16,"test kick",ALIGN::RIGHT);
  if(ui.buttonUp(1)) kicker.kick();
  
  return;
}

void Display::Line(){
  printd(8,8,"Line");

  display.drawCircle(64, 32, 24, WHITE);
  display.drawPixel(64, 32, WHITE);

  // エンジェル
  uint8_t circle_r = 16;
  for(int i=0;i<26;i++){
    float angle = radians(-i*360/26.0 + 180);
    if(line.line[i]) display.drawPixel(DISPLAY_W/2+cos(angle)*circle_r, DISPLAY_H/2+sin(angle)*circle_r, WHITE);
  }

  // エンジェル外側
  uint8_t outside_r = 20;
  for(int i=0;i<4;i++){
    float angle = radians(-i*90 + 180);
    if(line.line[i+26]) display.drawPixel(DISPLAY_W/2+cos(angle)*outside_r, DISPLAY_H/2+sin(angle)*outside_r, WHITE);
  }

  // ベクトル
  display.drawRect(64-1-cos(radians(line.dir))*13, 32-1+sin(radians(line.dir))*13, 2, 2, WHITE);

  // 情報
  printd(8, 40, "on:"+to_string(line.on));
  printd(8, 48, "num:"+to_string(line.num));
  printd(8, 56, "dir:"+to_string(line.dir));

  // しきい値調整
  printd(112,8, "+");
  printd(112,32,"-");

  // しきい値の加減算
  if(ui.buttonUp(1)){
    // line.send('u');
    Serial1.print("u");
    printd(96,8, "+");
  } 
  if(ui.buttonUp(2)){
    // line.send('d');
    Serial1.print("d");
    printd(96,32,"-");
  } 

  return;
}

void Display::Motor(){
  printd(8,8,"Motor");

  printd(16,16,"m1   m2   m3   m4");
  int width = (128-8*2) / 4;
  for(int i=0;i<4;i++){
    display.drawRect(8+i*width, 24,width,32,WHITE);
    int y1 = 40;
    int y2 = abs(motor.motor_raw[i]) * 16 / 100.0f;
    if(motor.motor_raw[i]<0) swap(y1,y2);
    // else                     y1 = y2 + y1;
    display.fillRect(8+i*width, y1, width, y2, WHITE);
  }

  return;
}

void Display::Valiables(){
  printd(8,8,"Valiables");

  // 表示
  for(size_t i=0;i<variables.size();i++){
    printd(16,16+8*i,variables[i]);
  }

  
  // 変数のセレクタ
  if(ui.buttonUp(0)){
    selector++;
    selector %= variables.size();
  }
  printd(8,16+selector*8,">");

  printd(120,8,"+",ALIGN::RIGHT);
  printd(120,32,"-",ALIGN::RIGHT);
  printd(128,56,"select",ALIGN::RIGHT);

  // 加減算
  if(ui.buttonUp(1))      *valiables_addr[selector] += 0.1;
  else if(ui.buttonUp(2)) *valiables_addr[selector] -= 0.1;

  return;
}

void Display::Game(){
  printd(8,8,"Game");

  // 攻め方向をリセット
  printd(120,8,"reset dir",ALIGN::RIGHT);
  if(ui.buttonUp(1)) dir.setDefault();

  printd(120,32,"change atk",ALIGN::RIGHT);
  printd(120,40,"sig="+to_string(camera.atk_sig),ALIGN::RIGHT);
  if(ui.buttonUp(2)) camera.changeAtk();


  printd(8,16,"damaged:");
  printd(16,24,to_string(ui.damaged_timer/1000) );
  if(ui.damaged_timer < 0) display.invertDisplay(true);


  return;
}
