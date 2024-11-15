#include "Line.hpp"
#include "Motor.hpp"
#include "UI.hpp"

constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;

bool line[INNER_NUM+4]{0};

constexpr uint8_t DATA_SIZE = 5;
constexpr uint8_t str_size = 8;
char str[str_size] = "";

class Vec2{
public:
  float x=0.0f, y=0.0f;
  Vec2(float m_x, float m_y){
    x = m_x;
    y = m_y;
  }

  float len(){
    return std::sqrt(x*x + y*y);
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  Serial1.begin(115200);
}

void loop() {

  // 余分な部分を読み飛ばす
  while(Serial1.available()>str_size){
    Serial1.read();
  }
  // 先頭の文字が来るまで読む
  while(Serial1.available()){
    if(Serial1.read() == 0b00011111) break;
  }
  while(Serial1.available()<=str_size-3){int i=0;}

  for(int i=0;i<6;i++){
    char c = Serial1.read();
    for(int j=0;j<5;j++){
      line[i*5+j] = (c>>(4-j)) & 0b00000001;
      Serial.print(line[i*5+j]);
    }
    Serial.print(" ");
  }
  Serial.print("  ");
  // Serial.println();

  // 壊れたセンサを反応しいないように修正
  line[0] = false;
  line[14-1] = false;
  line[24-1] = false;

  int line_num = 0;
  Vec2 line_vec(0.0f, 0.0f);
  float line_dir = 0;
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]){
      float sensor_dir = radians(i*360/INNER_NUM);
      line_vec.x += cos(sensor_dir);
      line_vec.y += sin(sensor_dir);
      line_num++;
    }
  }
  line_dir = -atan2(line_vec.y, line_vec.x);

  Serial.printf("dir:%f len:%f\n", degrees(line_dir), line_vec.len());

  if(line_num == 0) {
    motor[0] = 0;
    motor[1] = 0;
    motor[2] = 0;
    motor[3] = 0;
  }else if(!digitalRead(SW_TAC_PIN[0])){
    moveDir(degrees(line_dir)+180, 60, 100);
  }else{
    motor[0] = 0;
    motor[1] = 0;
    motor[2] = 0;
    motor[3] = 0;
  }

  motorP();
  motorRaw();

  delay(50);
}