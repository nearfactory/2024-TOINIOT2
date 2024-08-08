#define PH 2
#define EN 3

void setup() {
  // pinMode(2, OUTPUT); // EN=PWM
  analogWriteFrequency(EN, 40000);
  pinMode(PH, OUTPUT); // PH=GPIO

  Serial.begin(115200);
  Serial.println("2024-08-06 Teensy M4 Test");
}

float n = 0;

void loop() {
  if(sin(n) >= 0){
    digitalWrite(PH, 0);
    analogWrite(EN, (int)(sin(n)*127));
  }
  else{
    digitalWrite(PH, 1);
    analogWrite(EN, (int)(sin(n)*-127));
  }

  n += 0.02;
  Serial.println((int)(sin(n)*127));

  delay(50);
}