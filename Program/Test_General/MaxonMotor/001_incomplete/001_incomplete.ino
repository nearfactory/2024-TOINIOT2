// 2 = EN
// 3 = PH

#define PI 3.14159265

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.begin(115200);
  Serial.println("Start");

  // analogWriteFrequency(2, 184999*2*2*2*2*2);
}

float theta = 0;

void loop() {
  analogWrite(2, abs(sin(theta/180*PI)*255));
  if(sin(theta/180*PI)*255 < 0){
    digitalWrite(3, 1);
  }
  else{
    digitalWrite(3, 0);
  }
  Serial.print(-255);
  Serial.print("\t");
  Serial.print(255);
  Serial.print("\t");
  Serial.println(sin(theta/180*PI)*255);
  theta += 1;
  delay(50);

}