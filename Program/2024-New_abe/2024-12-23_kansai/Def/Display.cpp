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

  // variables.clear();
  // valiables_addr.clear();
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

void Display::printd(uint8_t x, uint8_t y, const char* str, ALIGN align_x, ALIGN align_y){
  switch(align_x){
    case ALIGN::LEFT :
      break;
    case ALIGN::CENTER :
      x -= strlen(str)*8/2;
      break;
    case ALIGN::RIGHT :
      x -= strlen(str)*8;
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
  display.print(str);

  return;
}



// void Display::addValiables(std::string name, float* addr){
//   variables.emplace_back(name);
//   valiables_addr.emplace_back(addr);
//   return;
// }



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
    // case MODE::VARIABLES :
    //   this->Valiables();
    //   break;
    default:
      // for(size_t i=0;i<variables.size();i++){
      //   printd(8,24+i*8,variables[i]);
      // }
      printd(8,8,"internal error!");
      break;
  }

  return;
}


void Display::Ball(){
  printd(8,8,"Ball");
  uint8_t circle_r = 24;
  uint8_t text_r = circle_r + 4;

  char str[100] = "";
  sprintf(str,"%4f",ball.dir);
  // string str = to_string(ball.dir);
  // str.erase(str.begin()+4,str.end());
  printd(8, 40, str);

  char buf[100] = "";
  sprintf(buf,"exist:%d",ball.is_exist);
  printd(8,48,buf);
  sprintf(buf,"hold :%d",ball.is_hold);
  printd(8,56,buf);
  
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, ball.dir+180, circle_r);
  drawAngleLine(DISPLAY_W/2, DISPLAY_H/2, 0, 8);

  sprintf(buf,"%d",ball.distance);
  printd(120,56,buf, ALIGN::RIGHT);
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
  char str[100] = "";
  sprintf(str, "%4f", dir.dir);
  // string str = to_string(dir.dir);
  // str.erase(str.begin()+5,str.end());
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
  display.drawCircle(64, 32, 30, WHITE);
  // display.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
  
  for(int i=0;i<4;i++){
    if(line.line[i])    display.drawPixel(64-4 - i*4, 32, WHITE);
  }
  for(int i=0;i<6;i++){
    if(line.line[i+4])  display.drawPixel(64, 32+4 + i*4, WHITE);
  }
  for(int i=0;i<6;i++){
    if(line.line[i+10]) display.drawPixel(64+4 + i*4, 32, WHITE);
  }
  for(int i=0;i<6;i++){
    if(line.line[i+16]) display.drawPixel(64, 32-4 -i*4, WHITE);
  }

  char str[100] = "";
  sprintf(str, "on :%d", line.on);
  printd(8, 48, str);
  sprintf(str, "num:%d", line.num);
  printd(8, 56, str);
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
    if(motor.motor_raw[i]<0){
      // swap(y1,y2);
      int buf = y1;
      y1 = y2;
      y2 = buf;
    } 
    // else                     y1 = y2 + y1;
    display.fillRect(8+i*width, y1, width, y2, WHITE);
  }

  return;
}

// void Display::Valiables(){
//   printd(8,8,"Valiables");
  
//   if(ui.buttonUp(3)){
//     selector++;
//     selector %= variables.size();
//   }

//   if(ui.buttonUp(1))      *valiables_addr[selector] += 0.1;
//   else if(ui.buttonUp(2)) *valiables_addr[selector] -= 0.1;

//   for(size_t i=0;i<variables.size();i++){
//     printd(16,16+8*i,variables[i]);
//   }
//   printd(8,16+selector*8,">");

//   printd(120,8,"+",ALIGN::RIGHT);
//   printd(120,32,"-",ALIGN::RIGHT);
//   printd(128,56,"select",ALIGN::RIGHT);

//   return;
// }