#include "Display.hpp"

using namespace std;

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
  // debug_variables_addr.clear();
  
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
  uint8_t text_r = circle_r + 4;

  string str = to_string(ball.dir);
  str.erase(str.begin()+4,str.end());
  printd(8, 40, str);

  printd(8,48,"exist:"+to_string(ball.is_exist));
  printd(8,56,"hold :"+to_string(ball.is_hold));
  
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, ball.dir+180, circle_r);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 0, 8);

  // double ball_small_angle = (ball_small_id*360.0/BALL_NUM+180.0)/180.0*3.14;
  // printd(DISPLAY_W/2+text_r*cos(-ball_small_angle), DISPLAY_H/2+text_r*sin(-ball_small_angle), to_string(ball_small), ALIGN::CENTER, ALIGN::MIDDLE);
  // double ball_big_angle = (ball_big_id*360.0/BALL_NUM+180.0)/180.0*3.14;
  // printd(DISPLAY_W/2+text_r*cos(-ball_big_angle), DISPLAY_H/2+text_r*sin(-ball_big_angle), to_string(ball_big), ALIGN::CENTER, ALIGN::MIDDLE);

  // printd(120,40,to_string(ball_small),    ALIGN::RIGHT);
  // printd(120,48,to_string(ball_big),      ALIGN::RIGHT);
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
  printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);
  return;
}

void Display::Dir(){
  printd(8,8,"Dir");

  uint8_t circle_r = 24;
  string str = to_string(dir.dir);
  str.erase(str.begin()+5,str.end());
  printd(8, 32, str);

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
  printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);
  return;
}

void Display::Line(){
  printd(8,8,"Line");
  uint8_t circle_r = 16;
  uint8_t outside_r = 20;

  display.drawCircle(64, 32, 24, WHITE);
  display.drawPixel(64, 32, WHITE);

  // エンジェル
  for(int i=0;i<26;i++){
    float angle = radians(-i*360/26.0 + 180);
    if(line.line[i]) display.drawPixel(DISPLAY_W/2+cos(angle)*circle_r, DISPLAY_H/2+sin(angle)*circle_r, WHITE);
  }
  // エンジェル外側
  for(int i=0;i<4;i++){
    float angle = radians(-i*90 + 180);
    if(line.line[i+26]) display.drawPixel(DISPLAY_W/2+cos(angle)*outside_r, DISPLAY_H/2+sin(angle)*outside_r, WHITE);
  }

  display.drawRect(64-1+line.vec.x*-13, 32-1+line.vec.y*13, 2, 2, WHITE);

  printd(8, 48, "on:"+to_string(line.on));
  printd(8, 56, "dir:"+to_string(line.dir));

  return;
}

void Display::Motor(){
  printd(8,8,"Motor");
  /*
  */
  printd(8,   24, "m4:"+to_string((int)motor.motor_raw[3]) );
  printd(120, 24, "m3:"+to_string((int)motor.motor_raw[2]), ALIGN::RIGHT);
  printd(8,   56, "m1:"+to_string((int)motor.motor_raw[0]) );
  printd(120, 56, "m2:"+to_string((int)motor.motor_raw[1]), ALIGN::RIGHT);

  // drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, move_dir, 24);

  return;
}

void Display::Valiables(){
  printd(8,8,"Valiables");
  
  if(ui.buttonUp(3)){
    selector++;
    selector %= variables.size();
  }
  printd(8,24+selector*8,">");

  if(ui.buttonUp(1))      *valiables_addr[selector] += 0.1;
  else if(ui.buttonUp(2)) *valiables_addr[selector] -= 0.1;

  for(size_t i=0;i<variables.size();i++){
    printd(16,24+8*i,variables[i]);
  }

  printd(120,8,"+",ALIGN::RIGHT);
  printd(120,32,"-",ALIGN::RIGHT);
  printd(128,56,"select",ALIGN::RIGHT);

  return;
}