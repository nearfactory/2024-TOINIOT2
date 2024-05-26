//ESP32C3(スレーブ)側コード

#include<Wire.h>

using uint8_t = unsigned char;
constexpr uint8_t ESP32C3_ADDR = 0x32;

int send_ms=0;
int receive_ms=0;

std::string send_str="";
std::string receive_str="";

void setup() {
  Serial.begin(9600);
  Serial.println("ESP32C3");

  Wire.begin(ESP32C3_ADDR);                             // ESP32C3_ADDR(0x32)としてI2Cを開始
  Wire.onReceive(receiveEvent);                         // マスターからの受信時のコールバック関数を設定
  Wire.onRequest(requestEvent);                         // マスターからの送信リクエスト時のコールバック関数を設定
}

void loop() {
  // 送受信結果を出力
  Serial.printf("Send : \"%s\"(%dms),  Receive : \"%s\"(%dms) \n", send_str.c_str(), send_ms, receive_str.c_str(), receive_ms);
  delay(100);
}

// マスターからの送信に対するコールバック関数
void receiveEvent(int size){
  int previous_receive_ms = millis();                   // I2Cの受信開始時刻を取得

  receive_str = "";                                     // 受信文字列をクリア
  while(Wire.available()){                              // 受信バッファが空になるまで繰り返す
    char c = Wire.read();                               // 受信バッファから1文字取り出す
    receive_str += c;                                   // 受信文字列の末尾に追加
  }

  receive_ms = millis() - previous_receive_ms;          // 開始時刻との差分を取って受信にかかった時間を計測

  return;
}

// マスターからのリクエストに対するコールバック関数
void requestEvent(){
  int previous_send_ms = millis();                      // リクエストの開始時刻を取得
  static int count=0;                                   // カウンタを宣言
  
  send_str = "ESP32C3 : " + std::to_string(count/2);    // 送信文字列を準備
  auto length = static_cast<char>(send_str.length());   // 文字列の長さをchar型(文字)として取得
  send_str =  length + send_str;                        // 送信文字列の先頭に、上の文字数を示すデータを追加
  count++;                                              // カウンタを1増やす
  Wire.write(send_str.c_str());                         // 送信バッファに追加

  send_ms = millis() - previous_send_ms;                // 開始時刻との差分を取ってリクエストの処理にかかった時間を計測

  return;
}