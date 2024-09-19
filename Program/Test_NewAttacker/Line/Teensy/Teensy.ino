#include <string>

char line[5]="";

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  Serial1.begin(115200);
  Serial1.println("Teensy");
  
  /*
  while(true){
    if(Serial1.available())
      receive_str += Serial1.read();

    char atmega[7]="AtMega";
    for(int i=0;i<7;i++){
      if(receive_str[i] != atmega[i])
        break;
    }
  }
  */

}

// 0b10101010, <>, <>, <>, <>, 0b01010101, \0
// 開始コード 0b10101010
// 終了コード 0b01010101

void loop() {
  if(Serial1.available()){
    while(Serial11.available()){
      if(Serial1.read()==0b10101010)
        break;
    }

    for(int i=0;i<6;i++){
      if(i<4) line[i] = c;
      while(!Serial1.available())
    }

  }

  for(int i=0;i<4;i++){
    for(int j=0;j<8;j++){
      char c = line[i];
      // リトルエンディアン
      Serial.print(c<<j & 0b00000001);
    }
  }
  Serial.println();

  delay(100);
}