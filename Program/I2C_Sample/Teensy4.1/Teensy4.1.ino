#include<string>

#include<Wire.h>

constexpr uint8_t ESP32C3_ADDR = 0x32;

void setup() {
  Serial.begin(9600);
  Serial.println("Teensy4.1");

  Wire.begin();
}

void loop() {
  static int count=0;
  count++;
  std::string send_str = "Teensy4.1 : " + std::to_string(count);

  // スレーブへデータを送信
  int previous_send_ms = millis();
  Wire.beginTransmission(ESP32C3_ADDR);
  Wire.write(send_str.c_str());
  Wire.endTransmission();
  int send_ms = millis() - previous_send_ms;

  // スレーブへデータをリクエスト
  int previous_receive_ms = millis();
  std::string receive_str="";
  Wire.requestFrom(ESP32C3_ADDR,128);
  unsigned char received_size = Wire.read();
  for(int i=0;i<received_size;i++){
    char c = Wire.read();
    receive_str += c;
    if(c=="\0") break;
  }
  int receive_ms = millis() - previous_receive_ms;

  // 送信・受信したデータを出力
  Serial.printf("Send : \"%s\"(%dms),  Receive : \"%s\"(%dms) \n", send_str.c_str(), send_ms, receive_str.c_str(), receive_ms);

  delay(100);
}