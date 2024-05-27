/*

MaxonモーターのPWM周波数目安は
39KHz - 60KHz
(https://maxonjapan.com/book/_004/ より)

2 = EN
3 = PH

*/

#define PI 3.14159265

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.begin(115200);
  Serial.println("Start");

  // PWM周波数を50KHzに設定
  // analogWriteFrequency(ピン番号, 周波数[Hz]);
  analogWriteFrequency(3, 50000);
}

float theta = 0;

void loop() {
  // 回転は常に100%
  digitalWrite(2, 1);

  float output;
  output = (sin(theta/180*PI)*100 + 100) / 2 / 100 * 255;
  // 正逆転をPWM制御する（0=逆転100%、127=ストップ、255=正転100%）
  analogWrite(3, (int)output);

  Serial.print(-100);
  Serial.print("\t");
  Serial.print(0);
  Serial.print("\t");
  Serial.print(100);
  Serial.print("\t");
  Serial.print(output);
  Serial.print("\t");
  Serial.println(sin(theta/180*PI)*100);

  theta += 1;

  delay(50);
}