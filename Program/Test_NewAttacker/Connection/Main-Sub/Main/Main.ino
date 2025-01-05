void setup() {
  Serial.begin(115200);
  Serial7.begin(115200);

}

#define STR_SIZE 4

void loop() {
  // 必要な分のデータを受信していない場合処理を飛ばす
  if(Serial7.available()<STR_SIZE){
    return;
  }
    
  // // 古い情報を読み飛ばす
  while(Serial7.available()>STR_SIZE){
    Serial7.read();
  }

  uint8_t str[4] = "";
  str[0] = Serial7.read();
  str[1] = Serial7.read();
  str[2] = Serial7.read();
  str[3] = Serial7.read();

  Serial.printf("min:0 max:255 b01k:%d b02k:%d vol:%d \n", str[0], str[1], str[2]);

  Serial7.print('k');

  delay(10);
}
