#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

Adafruit_SSD1306 display(128, 64, &Wire2, -1);

// motor
constexpr uint8_t MOTOR_NUM = 4;
constexpr uint8_t MOTOR_PWM_TYPE = 2;
const uint8_t MOTOR_PIN[MOTOR_NUM][MOTOR_PWM_TYPE]{
  {29, 28},
  {9, 6},
  {5, 4},
  {3, 2}
};
enum MOTOR : uint8_t{
  EN = 0,
  PH
};

// osushi
constexpr uint8_t OSUSHI_PIN = 32;

// ball
constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  // 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
  A0, A1, A2, A3, A4, A5, A6, A7, A8, A9,  A12, A13, A14, A15, A16, A17
};

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Shirahama Test 2024-08-02");

  Wire2.begin();
  while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c));
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(8,8);
  display.print("Shirahama test");
  display.display();

  // motor
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
    pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 40000);
  }

  // osushi
  pinMode(OSUSHI_PIN, INPUT);

  // ball
  for(auto p:BALL_PIN) pinMode(p, INPUT);
  
  delay(256);
}

void loop() {
  display.clearDisplay();
  /*
  */
  // motor
  uint8_t power = 128;
  Serial.print("motor : ");
  for(int i=0;i<MOTOR_NUM;i++){
    analogWrite(MOTOR_PIN[i][MOTOR::EN], power);
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], 0);
    Serial.printf("%04d ", power);
  }
  Serial.println();

  // osushi
  static auto previous_kick_ms = millis();
  if( millis()-previous_kick_ms > 5000){
    digitalWrite(OSUSHI_PIN, 1);
    delay(100);
    digitalWrite(OSUSHI_PIN, 0);

    previous_kick_ms = millis();
    Serial.println("kick!");
  }else{
    digitalWrite(OSUSHI_PIN, 0);
  }
  // Serial.println(digitalRead(OSUSHI_PIN));

  // ball
  // Serial.print("ball  : ");
  short ball[BALL_NUM] = { 1023 };
  for(int p=0;p<BALL_NUM;p++){
    // Serial.printf("ball%02d%04d ", p, analogRead(BALL_PIN[p]));
    ball[p] = analogRead(BALL_PIN[p]);
    Serial.printf("%04d ", ball[p]);
  }
  Serial.println();
  
  // 全てのセンサの値
  short ball_x = 0;
  short ball_y = 0;
  double ball_dir = 0.0;

  for(int i=0;i<BALL_NUM;i++){
    double sensor_dir = (i-1)*360.0/16.0 + 180.0 - 67.5;
    // Serial.print(":nsensor:");
    // Serial.print(sensor_dir);
    ball_x += (1023 - ball[i]) * cos(sensor_dir/180.0*3.14);
    ball_y += (1023 - ball[i]) * sin(sensor_dir/180.0*3.14);
  }
  ball_dir = atan2(ball_y, ball_x) * 180.0 / 3.14 - 90.0;
  ball_dir = 180-ball_dir;
  Serial.println(ball_dir);

  short r = 24;
  display.drawLine(64, 32, 64+cos(ball_dir*3.14/180.0)*r, 32+sin(ball_dir*3.14/180.0)*r, WHITE);

  display.display();
  delay(50);
}
