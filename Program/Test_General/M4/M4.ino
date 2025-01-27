// New-Robot-002
// M1: PH08 EN09
// M2: PH06 EN07
// M3: PH04 EN05
// M4: PH02 EN03

#define PH 4
#define EN 5

void setup() {
  analogWriteFrequency(EN, 4000); // EN=PWM
  pinMode(PH, OUTPUT); // PH=GPIO

  Serial.begin(115200);
  Serial.println("2024-08-06 Teensy M4 Test");
}

float n = 0;

void loop() {
  if(sin(n) >= 0){
    digitalWrite(PH, 0);
    analogWrite(EN, (int)(sin(n)*255));
  }
  else{
    digitalWrite(PH, 1);
    analogWrite(EN, (int)(sin(n)*-255));
  }

  n += 0.02;
  Serial.println((int)(sin(n)*100));

  delay(50);
}