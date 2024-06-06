const uint8_t LINE_PIN[22] = { 26, 27, 28, 29, 42, 43, 44, 45, 46, 47, 36, 37, 38, 39, 40, 41, 30, 31, 32, 33, 34, 35 };
const uint8_t BALL_PIN[16] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };
const uint8_t MOTOR_PIN[4][2] = { { 19, 3 }, { 17, 2 }, { 25, 5 }, { 23, 4 } };

void setup() {
  Serial.begin(9600);
  Serial.println("IOT2 2024-06-06 AtMega2560 Test");
  for(auto n:LINE_PIN) pinMode(n,INPUT);
  for(auto n:BALL_PIN) pinMode(n,INPUT);
}

void loop() {
  static int count=0;
  static int processing_start = 0;
  processing_start = millis();

  // line
  Serial.print("line:");
  for(auto x:LINE_PIN) Serial.print(digitalRead(x));
  Serial.println();

  // ball
  Serial.print("ball:");
  for(auto n:BALL_PIN){
    Serial.print(analogRead(n));
    Serial.print("\t");
  }
  Serial.println("\n");

  // motor
  static int power=0;
  power = sin(count/80.0)*255;
  for(int i=0;i<4;i++){
    digitalWrite(MOTOR_PIN[i][0], power<0?0:1 );
    analogWrite(MOTOR_PIN[i][1], abs(power));
  }

  Serial.print("processing time:");
  Serial.print(millis()-processing_start);
  Serial.println("(ms)");
  Serial.println("\n");

  count++;
  delay(50);
}