// New-Robot-002
// M1: PH08 EN09
// M2: PH06 EN07
// M3: PH04 EN05
// M4: PH02 EN03

#define PH 4
#define EN 5

void setup() {
  analogWriteFrequency(PH, 4000); // PH=PWM
  pinMode(EN, OUTPUT); // EN=GPIO

  Serial.begin(115200);
  Serial.println("2024-08-06 Teensy M4 Test");
}

float n = 0;

void loop() {
  digitalWrite(EN, 1);
  analogWrite(PH, (int)(sin(n)*127 + 127));
  Serial.print((int)(sin(n)*100));
  Serial.print("\t");
  Serial.print(100);
  Serial.print("\t");
  Serial.println(-100);

  n += 0.02;

  delay(50);
}