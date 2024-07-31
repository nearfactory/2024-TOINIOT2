// メイン基板動作確認 Teensy4.1 2024-07-31

// 使い方
// 変数:BALL_PIN にはんだ付けしたボールセンサのピン番号を代入する
// 一旦、UART・I2C はコメントアウトしている

// 確認項目
// ・ボールセンサの値が正常に表示される
// ・BNO055の値が正常に表示される

// ・UART:"ESP32C3 UART" と表示される
// ・I2C:"ESP32C3 I2C" と表示される

// ※そもそもこのテストプログラム自体が間違っている可能性がある

#include <Adafruit_BNO055.h>
#include <Wire.h>

constexpr uint8_t BALL_PIN = A0;

Adafruit_BNO055 bno(55,0x28,&Wire2);
constexpr uint8_t ESP_ADDR = 0x32;

void setup() {
  // Serial
  Serial.begin(9600);
  Serial.println("TOINIOT2 Main Teensy4.1 Test 2024-07-31");

  /*
  // Serial2 (to sub)
  Serial2.begin(9600);
  Serial2.println("Teensy4.1");
  */

  // I2C
  Wire2.begin();

  // ball
  pinMode(BALL_PIN, INPUT);

  // bno
  if(!bno.begin()){
    Serial.println("bno error");
    while(true);
  }

}

void loop() {
  auto begin_ms = millis();
  /*
  // Serial2 (to sub)
  Serial2.print("Teensy4.1 UART");
  Serial.print("UART:\"");
  while(Serial2.available()){
    Serial.print( static_cast<char>(Serial2.read()) );
  }
  Serial.print("\"\n");

  // I2C
  Wire2.beginTransmission(ESP_ADDR);
  Wire2.write("Teensy4.1 I2C");
  Wire2.endTransmission();

  Serial.print("I2C:\"");
  Wire2.requestFrom(ESP_ADDR, 12);
  for(int i=0;i<12;i++){
    Serial.print( static_cast<char>(Wire2.read()) );
  }
  Serial.print("\"\n");
  */

  // ball
  short ball = analogRead(BALL_PIN);
  Serial.printf("ball:%d\n", ball);

  // bno
  sensors_event_t dir{};
  bno.getEvent(&dir, Adafruit_BNO055::VECTOR_EULER);
  Serial.printf("BNO055 x:%f, y:%f, z:%f\n", dir.orientation.x, dir.orientation.y, dir.orientation.z);

  // ----
  Serial.printf("process:%d(ms)\n", millis()-begin_ms );

}