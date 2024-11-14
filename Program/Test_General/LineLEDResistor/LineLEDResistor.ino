void setup() {
  Serial.begin(115200);
  Serial.println("20241114 TOINOT2 Linesensor-Test");
}

void loop() {
  for(int i=0; i<100; i++){
    Serial.print(analogRead(A0));
    Serial.print(",");
    delay(100);
  }
  while(1){
    Serial.println();
    delay(100);
  }
}
