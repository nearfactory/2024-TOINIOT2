#include <string>

std::string receive_str{};

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  Serial1.begin(115200);
  Serial1.println("Teensy");
  
  while(true){
    if(Serial1.available())
      receive_str += Serial1.read();

    if(receive_str == "AtMega")
      break;
  }

}

void loop() {
  if(Serial1.available()){
    receive_str = "";
    while(Serial1.available()){
      receive_str += Serial1.read();
    }
    Serial1.println("T");
  }

  if(receive_str.length()!=4){
    Serial.println("err!");
  }

  for(int i=0;i<receive_str.length();i++){
    for(int j=0;j<8;j++){
      char c = receive_str[i];
      // リトルエンディアン
      Serial.print(c<<j & 0b00000001);
    }
  }
  Serial.println();

  delay(100);
}