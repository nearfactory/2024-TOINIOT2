void setup(){
  Serial.begin(9600);              // シリアル通信の開始(ボーレート9600bps)
  Serial2.begin(9600);              // シリアル通信の開始(ボーレート9600bps)

  Serial.println("Start Debugging");
}

void loop(){
  while(Serial2.available() > 0){
    char val = Serial2.read();       // 受信したデータを読み込む
    Serial.println(val);
    if(val == '1'){                // "1"ならLEDを消灯、"0"ならLEDを点灯
      digitalWrite(13, HIGH);
      // Serial.println(1);
    } else if(val == '0'){
      digitalWrite(13, LOW);
      // Serial.println(0);
    }
  }
  // Serial.println("aaa");
  delay(50);
}