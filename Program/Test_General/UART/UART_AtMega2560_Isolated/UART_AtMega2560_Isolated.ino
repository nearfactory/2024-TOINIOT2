void setup(){
  Serial.begin(9600);              // シリアル通信の開始(ボーレート9600bps)
}

void loop(){
  // Serial.write('1');
  // Serial.write('1');
  // Serial.write('1');
  // Serial.write('1');
  // Serial.write('1');
  Serial.print(1);
  Serial.print(1);
  Serial.print(1);
  Serial.print(1);
  Serial.print(1);
  delay(1000);
  // Serial.write('0');
  // Serial.write('0');
  // Serial.write('0');
  // Serial.write('0');
  // Serial.write('0');
  Serial.print(0);
  Serial.print(0);
  Serial.print(0);
  Serial.print(0);
  Serial.print(0);
  delay(1000);
}