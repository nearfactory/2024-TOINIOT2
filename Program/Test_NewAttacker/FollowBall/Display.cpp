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
  // debug_variables_addr.clear();
  
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



template <typename T>
void Display::add(std::string name, T variables){
  variables.emplace_back(name+":"+to_string(variables));
  return;
}



void Display::debug(uint8_t mode){
  switch(mode){
    case MODE::BALL :
      this->ball();
      break;
    case MODE::BLE :
      this->ble();
      break;
    case MODE::CAMERA :
      this->camera();
      break;
    case MODE::DIR :
      this->dir();
      break;
    case MODE::DRIBBLER :
      this->dribbler();
      break;
    case MODE::KICKER :
      this->kicker();
      break;
    case MODE::LINE :
      this->line();
      break;
    case MODE::MOTOR :
      this->motor();
      break;
    case MODE::VARIABLES :
      this->valiables();
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


void Display::ball(){
  /*
  uint8_t circle_r = 24;
  uint8_t text_r = circle_r + 4;

  string str = to_string(ball_dir);
  str.erase(str.begin()+4,str.end());
  printd(8, 40, str);

  // printd(8,24,"dist:",ALIGN::LEFT,ALIGN::BOTTOM);
  printd(8,48,"exist:"+to_string(ball_exist));
  printd(8,56,"hold :"+to_string(ball_holding));
  
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, -ball_dir-180, circle_r);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 0, 8);

  // double ball_small_angle = (ball_small_id*360.0/BALL_NUM+180.0)/180.0*3.14;
  // printd(DISPLAY_W/2+text_r*cos(-ball_small_angle), DISPLAY_H/2+text_r*sin(-ball_small_angle), to_string(ball_small), ALIGN::CENTER, ALIGN::MIDDLE);
  // double ball_big_angle = (ball_big_id*360.0/BALL_NUM+180.0)/180.0*3.14;
  // printd(DISPLAY_W/2+text_r*cos(-ball_big_angle), DISPLAY_H/2+text_r*sin(-ball_big_angle), to_string(ball_big), ALIGN::CENTER, ALIGN::MIDDLE);

  printd(120,40,to_string(ball_small),    ALIGN::RIGHT);
  printd(120,48,to_string(ball_big),      ALIGN::RIGHT);
  printd(120,56,to_string(ball_distance), ALIGN::RIGHT);
  for(int i=0;i<BALL_NUM;i++){
    double angle = (i*360/BALL_NUM+180);
    // Serial.println(angle);
    angle = angle/180*3.14;
    uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
    uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
    // printd(x, y, ".", ALIGN::LEFT, ALIGN::BOTTOM);
    display.drawPixel(x, y, WHITE);
    // printd(DISPLAY_W/2+cos(angle)*circle_r, DISPLAY_H/2+sin(angle)*circle_r, to_string(ball[i]),ALIGN::CENTER, ALIGN::MIDDLE);
  }
  */

  return;
}

void Display::ble(){
  /*
  printd(8, 24, "ATK:"+BLE_atk);
  printd(8, 40, "DEF:"+BLE_def);
  */

  return;
}

void Display::camera(){
  printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);
  return;
}

void Display::dir(){
  /*
  uint8_t circle_r = 24;
  string str = to_string(dir);
  str.erase(str.begin()+5,str.end());
  printd(8, 32, str, ALIGN::LEFT, ALIGN::MIDDLE);

  // display.drawPixel(DISPLAY_W/2+24*cos((180-prev_dir)*3.14/180.0),DISPLAY_H/2+24*sin((180-prev_dir)*3.14/180.0),WHITE);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 180, 24);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 180-dir, 16);

  display.drawPixel(DISPLAY_W/2+accel_x*6, DISPLAY_H/2+accel_y*6, WHITE);

  for(int i=0;i<8;i++){
    double angle = i*360/8 -dir;
    angle = angle/180*3.14;
    uint8_t x = DISPLAY_W/2+(int16_t)(cos(angle)*circle_r);
    uint8_t y = DISPLAY_H/2+(int16_t)(sin(angle)*circle_r);
    display.drawPixel(x, y, WHITE);
  }
  */
  
  return;
}

void Display::dribbler(){
  printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);
  return;
}

void Display::kicker(){
  printd(64,32,"no data", ALIGN::CENTER, ALIGN::MIDDLE);
  return;
}

void Display::line(){
  /*
  uint8_t circle_r = 20;
  uint8_t outside_r = 28;

  // エンジェル
  for(int i=0;i<LINE_CENTRAL;i++){
    double angle = (180+i)*360.0/LINE_CENTRAL*3.14/180.0;
    if(line[i]) display.drawPixel(DISPLAY_W/2+cos(angle)*circle_r, DISPLAY_H/2+sin(angle)*circle_r, WHITE);
  }
  // エンジェル外側
  for(int i=LINE_CENTRAL;i<LINE;i++){
    double angle = ((i-LINE_CENTRAL)/4*90+180 + (i-LINE_CENTRAL)%4*10-15)*3.14/180.0;
    if(line[i]) display.drawPixel(DISPLAY_W/2+cos(angle)*outside_r, DISPLAY_H/2+sin(angle)*outside_r, WHITE);
  }

  printd(8, 60, "on:"+to_string(line_on), ALIGN::LEFT, ALIGN::MIDDLE);
  printd(8, 68, "dir:"+to_string(line_dir), ALIGN::LEFT, ALIGN::MIDDLE);
  */

  return;
}

void Display::motor(){
  /*
  printd(8,   24, "m4:"+to_string((int)motor_raw[3]) );
  printd(120, 24, "m3:"+to_string((int)motor_raw[2]), ALIGN::RIGHT);
  printd(8,   56, "m1:"+to_string((int)motor_raw[0]) );
  printd(120, 56, "m2:"+to_string((int)motor_raw[1]), ALIGN::RIGHT);

  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, move_dir, 24);
  */

  return;
}

void Display::valiables(){
  for(size_t i=0;i<variables.size();i++){
    printd(16,24+8*i,variables[i]);
  }
  return;
}