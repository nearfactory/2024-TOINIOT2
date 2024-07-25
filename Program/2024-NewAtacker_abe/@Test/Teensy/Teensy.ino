
// Teensy4.1
// I2C Master

#include <string>

#include <Adafruit_BNO055.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Ball
constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
  // A0, A1, A2, A3, A4, A5, A6, A7, A8, A9,  A12, A13, A14, A15, A16, A17
};

// Camera Serial 8

// DIR
Adafruit_BNO055 bno(55, 0x28, &Wire);

// Dribbler (sub)

// Line Serial 1

// Motor
constexpr int MOTOR_NUM = 4;
constexpr int MOTOR_PWM_TYPE = 2;
const uint8_t MOTOR_PIN[MOTOR_NUM][MOTOR_PWM_TYPE]{
  {29, 28},
  {9, 6},
  {5,4},
  {3,2}
};
const uint8_t DEFAULT_MOTOR_DIR[MOTOR_NUM] = {
  1,1,1,1
};
enum MOTOR : uint8_t{
  LF = 0,
  LB,
  RB,
  RF,
  EN = 0,
  PH,
};

// ToSub Serial 2

// UI
constexpr int BUTTON_NUM = 4;
const uint8_t BUTTON_PIN[BUTTON_NUM] = {
  36, 37, 31, 30
};
constexpr int BUZZER_PIN = 33;
constexpr uint8_t DISPLAY_W = 128;
constexpr uint8_t DISPLAY_H = 64;
constexpr uint8_t DISPLAY_ADDR = 0x3c;
constexpr uint8_t OLED_RESET = -1;
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire, OLED_RESET);


void setup() {

  // Serial
  Serial.begin(9600);
  Serial.println("Teensy4.1 Test Program 2024-07-25");

  // Ball
  for(auto p:BALL_PIN) pinMode(p, INPUT);
  Serial.println("Ball");

  // Camera
  // ?

  // DIR
  while(!bno.begin());
  Serial.println("DIR");

  // Line
  /*
  Serial1.begin(9600);
  Serial1.println("Teensy4.1 Test Program 2024-07-25");
  Serial.println("Line");
  */

  // Motor
  for(int i=0;i<MOTOR_NUM;i++){
    for(int j=0;j<MOTOR_PWM_TYPE;j++)
      pinMode(MOTOR_PIN[i][j], OUTPUT);
  }
  Serial.println("Motor");

  // ToSub
  Serial2.begin(9600);
  Serial2.println("Teensy4.1 Test Program 2024-07-25");
  Serial.println("ToSub");

  // UI-Button
  for(auto p:BUTTON_PIN) pinMode(p, INPUT);

  // UI-Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // UI-Display
  Wire.begin();
  while(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR));
  display.setTextSize(8);

  Serial.println("UI");

  Serial.println("Finish setup() function!");
}

void loop() {

  static unsigned int count = 0;
  int begin_ms = millis();


  // Ball
  unsigned short ball[BALL_NUM]{1023};
  for(int i=0;i<BALL_NUM;i++) ball[i] = analogRead(BALL_PIN[i]);
  Serial.print("Ball\t:");
  for(auto b:ball) Serial.printf("%d ", b);
  Serial.println();

  // Camera
  // ? 

  // DIR
  sensors_event_t orientationData{};
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  double dir = orientationData.orientation.x;
  Serial.printf("Dir\t\t:%lf", dir);

  // Line
  /*
  Serial.print("Line\t:");
  while(Serial1.available()){
    Serial.print(static_cast<char>(Serial1.read()));
  }
  Serial.println();
  */

  // Motor
  uint8_t motor_power = sin(count/360.0)*100+100;
  for(int i=0;i<MOTOR_NUM;i++){
    analogWrite(MOTOR_PIN[i][MOTOR::EN], motor_power);
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], DEFAULT_MOTOR_DIR[i]);
  }
  Serial.printf("Motor\t:%d\n", motor_power);

  // ToSub-TX
  Serial.print("ToSub-TX\t:");
  std::string sub_tx{};
  for(auto b:ball) sub_tx += (std::to_string(b) + " ");
  Serial2.write(sub_tx.c_str());
  Serial.printf("\"%s\"\n", sub_tx.c_str());

  // ToSub-RX
  Serial.print("ToSub-RX\t:");
  std::string sub_rx{};
  while(Serial2.available()){
    sub_rx += static_cast<char>(Serial2.read());
  }
  Serial.printf("\"%s\"\n", sub_rx.c_str());

  // UI-Button
  Serial.print("Button\t:");
  bool button[BUTTON_NUM]{false};
  for(int i=0;i<BUTTON_NUM;i++) button[i] = digitalRead(BUTTON_PIN[i]);
  for(auto b:button) Serial.print(b);
  Serial.println();

  // UI-Buzzer
  float buzzer_tone = 261.626;
  analogWrite(BUZZER_PIN, buzzer_tone);
  Serial.printf("Buzzer\t:%f\n", buzzer_tone);

  // UI-Display
  display.clearDisplay();
  display.setCursor(8, 8);
  display.println("Teensy4.1 Test Program 2024-07-25");
  display.println();
  display.print("Button:");
  for(auto b:button) display.printf("%d ", b);
  display.display();

  count++;
  Serial.printf("Time\t:%d(ms)\n\n", millis()-begin_ms );

}
