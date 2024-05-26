//Teensy4.1(マスター)側コード

#include<string>

#include<Wire.h>

using uint8_t = unsigned char;
constexpr uint8_t ESP32C3_ADDR = 0x32;

void setup() {
  Serial.begin(9600);
  Serial.println("Teensy4.1");

  Wire.begin();
}

void loop() {
  static int count=0;
  count++;
  std::string send_str = "Teensy4.1 : " + std::to_string(count);  //送信データを準備

  // スレーブへデータを送信
  int previous_send_ms = millis();                  // I2Cの送信開始時刻を取得
  Wire.beginTransmission(ESP32C3_ADDR);             // ESP32C3との通信準備を開始
  Wire.write(send_str.c_str());                     // データを送信バッファに追加
  Wire.endTransmission();                           // 送信
  int send_ms = millis() - previous_send_ms;        // 開始時刻との差分を取って通信時間を計測

  // スレーブへデータをリクエスト
  int previous_receive_ms = millis();               // I2Cのリクエスト開始時刻を取得
  std::string receive_str="";                       // 受信データ格納用文字列を宣言
  Wire.requestFrom(ESP32C3_ADDR,128);               // ESP32C3への送信リクエスト
  uint8_t received_size = Wire.read();              // 文字列の長さを示すために1文字目に格納しておいたデータを読み出す
  for(int i=0;i<received_size;i++){                 // 上で読み出した長さの回数分繰り返す
    char c = Wire.read();                           // 受信バッファから文字として1つ読み出す
    receive_str += c;                               // 文字列の末尾に格納する
  }
  int receive_ms = millis() - previous_receive_ms;  // 開始時刻との差分を取ってリクエストの処理にかかった時間を計測

  // 送信・受信したデータを出力
  Serial.printf("Send : \"%s\"(%dms),  Receive : \"%s\"(%dms) \n", send_str.c_str(), send_ms, receive_str.c_str(), receive_ms);

  delay(100);
}