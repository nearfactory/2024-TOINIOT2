// メイン基板動作確認 ESP32C3 2024-07-31

// 成功条件
// ・UART:"Teensy4.1 UART" と表示される
// ・I2C:"Teensy4.1 I2C" と表示される

// ※そもそもこのテストプログラム自体が間違っている可能性がある

#include <Wire.h>

constexpr uint8_t ESP_ADDR = 0x32;

void setup() {
  // Serial
  Serial.begin(9600);
  Serial.println("TOINIOT2 Main ESP32C3 Test 2024-07-31");

  // Serial1 (to main)
  Serial1.setPins(D7, D6);
  Serial1.begin(9600);
  Serial1.println("ESP32C3");

  Wire.begin(ESP_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  Serial1.print("ESP32C3 UART");
  Serial.print("UART\"");
  while(Serial1.available()){
    Serial.print( static_cast<char>( Serial1.read()) );
  }
  Serial.print("\"\n");

  delay(100);
}

void receiveEvent(int size){
  Serial.print("I2C\"");
  while(Wire.available()){
    Serial.print( static_cast<char>(Wire.read()) );
  }
  Serial.print("\"\n");

  return;
}

void requestEvent(){
  Wire.print("ESP32C3 I2C");

  return;
}