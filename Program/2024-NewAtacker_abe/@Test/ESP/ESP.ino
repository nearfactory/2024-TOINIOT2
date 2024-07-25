
// ESP32C3
// I2C Slave

#include<cmath>

#include<Wire.h>

constexpr uint8_t ESP_ADDR = 0x32;
constexpr uint8_t BALL_01_K_PIN = 1;    // A0 D0
constexpr uint8_t BALL_02_K_PIN = 2;    // A1 D1
constexpr uint8_t DRIBBLER_PH_PIN = 3;  // A2 D2
constexpr uint8_t DRIBBLER_EN_PIN = 4;  // A3 D3
// 5 SDA D4
// 6 SCL D5
// 7 TX D6
// 8 RX D7
constexpr uint8_t NEOPIXEL_PIN = 11;    // MOSI D10

void setup() {
  // Serial
  Serial.begin(9600);
  Serial.println("ESP32C3 Test Program 2024-07-25");

  // Ball
  pinMode(BALL_01_K_PIN, INPUT);
  pinMode(BALL_02_K_PIN, INPUT);

  // Dribbler
  pinMode(DRIBBLER_PH_PIN, OUTPUT);
  pinMode(DRIBBLER_EN_PIN, OUTPUT);

  // I2C
  Wire.begin(ESP_ADDR);

  // NeoPixel
  pinMode(NEOPIXEL_PIN, OUTPUT);

  // ToMain
  Serial1.setPins(D7, D6);
  Serial1.begin(9600);
}

void loop() {
  static unsigned int count = 0;
  unsigned int begin_ms = millis();

  // Ball
  unsigned short ball01k = analogRead(BALL_01_K_PIN);
  unsigned short ball02k = analogRead(BALL_02_K_PIN);
  Serial.printf("Ball\t:%d %d", ball01k, ball02k);

  // Dribbler
  uint8_t motor_power = std::sin(count/360.0)*100+100;
  analogWrite(DRIBBLER_EN_PIN, motor_power);
  digitalWrite(DRIBBLER_PH_PIN, 0);
  Serial.printf("Dribbler\t:%f %d", motor_power, 0);

  // NeoPixel
  // ?

  // ToMain-TX
  Serial1.printf("ToMain-TX\t:\"%d %d\"\n", ball01k, ball02k);

  // ToMain-RX
  Serial.print("ToMain-RX\t:\"");
  while(Serial1.available()){
    Serial.print(static_cast<char>(Serial1.read()));
  }
  Serial.println("\"");

  count++;
  Serial.printf("Time\t:%d(ms)\n\n", millis()-begin_ms );
}
