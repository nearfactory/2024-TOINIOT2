#include<Wire.h>

constexpr uint8_t ESP32C3_ADDR = 0x32;

int send_ms=0;
int receive_ms=0;

std::string send_str="";
std::string receive_str="";

void setup() {
  Serial.begin(9600);
  Serial.println("ESP32C3");

  Wire.begin(ESP32C3_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  Serial.printf("Send : \"%s\"(%dms), Receive : \"%s\"(%dms) \n", send_str.c_str(), send_ms, receive_str.c_str(), receive_ms);
  delay(100);
}

// マスターからの送信に対するコールバック関数
void receiveEvent(int size){
  int previous_receive_ms = millis();

  receive_str = "";
  while(Wire.available()>0){
    char c = Wire.read();
    receive_str += c;
  }

  receive_ms = millis() - previous_receive_ms;

  return;
}

// マスターからのリクエストに対するコールバック関数
void requestEvent(){
  int previous_send_ms = millis();
  static int count=0;
  
  send_str = "ESP32C3 : " + std::to_string(count/2) + "\0";
  auto length = static_cast<char>(send_str.length());
  send_str =  length + send_str;
  count++;
  Wire.write(send_str.c_str());

  send_ms = millis() - previous_send_ms;

  return;
}