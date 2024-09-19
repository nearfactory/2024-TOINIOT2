void setup() {
  pinMode(2, OUTPUT); // PH
  pinMode(3, OUTPUT); // EN

  Serial.begin(115200);
  Serial.println("2024-08-06 ESP32C3 Dribbler Test");
}

float n = 0;

void loop() {
  if(sin(n) >= 0){
    digitalWrite(3, 0);
    analogWrite(2, (int)sin(n)*127);
  }
  else{
    digitalWrite(3, 1);
    analogWrite(2, (int)sin(n)*127);
  }

  n += 0.01;
  delay(10);
}
