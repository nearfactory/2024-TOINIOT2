// ESP32C3
// 2024-07-30

#include <Wire.h>

enum class PIN : uint8_t{
  BALL_R = 0,
  BALL_L,
  DRIBBLER_PH,
  DRIBBLER_EN,
  NEOPIXEL = 10,
};

constexpr uint8_t I2C_ESP_ADDR = 0x32;

void setup() {
  // PIN
  pinMode(PIN::BALL_R);
  pinMode(PIN::BALL_L);
  pinMode(PIN::DRIBBLER_PH);
  pinMode(PIN::DRIBBLER_EN);
  pinMode(PIN::NEOPIXEL);

  // Serial
  Serial.begin(9600);
  Serial.println("TOINIOT2 ESP32C3 2024-07-30");
  Serial1.begin(9600);
  Serial1.print("E");

  // I2C
  Wire.begin(I2C_ESP_ADDR);
}

void loop() {
  uint16_t ball_r = analogRead(PIN::BALL_R);
  uint16_t ball_l = analogRead(PIN::BALL_L);

  analogWrite(PIN::DRIBBLER_PH);
  analogWrite(PIN::DRIBBLER_EN);
}
