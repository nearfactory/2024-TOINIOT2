constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;
const     uint8_t LINE_PIN[LINE_NUM] = {
  7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
};

constexpr uint8_t ANALOG_NUM = 12;
const     uint8_t ANALOG_PIN[ANALOG_NUM] = {
  A11, A10, A2, A3, A1, A9, A8, A6, A0, A7, A5, A4
};

constexpr uint8_t THRESHOLD_PIN = 2;

bool line[LINE_NUM];
int analog[ANALOG_NUM];

void setup(){
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line 2024-10-12");

  for(auto p:LINE_PIN) pinMode(p, INPUT_PULLUP);
  // pinMode(6+2, INPUT);
  // pinMode(6+5, INPUT);
  // pinMode(6+14, INPUT);
  // pinMode(6+24, INPUT);
  // pinMode(6+38, INPUT);
  // pinMode(6+39, INPUT);
  // pinMode(6+40, INPUT);
  
  for(auto p:ANALOG_PIN) pinMode(p, INPUT_PULLUP);
  pinMode(THRESHOLD_PIN, OUTPUT);
}

// U1
// 常に1 U5 U14 U24 U38-40

uint8_t threshold = 162;

void loop(){
  if(Serial.available()){
    int i=0;
    char buf[100]{};
    for(auto &b:buf) b='0';
    while(Serial.available()){
      buf[i] = Serial.read();
      i++;
    }
    threshold = atoi(buf)/4;
  }
  analogWrite(THRESHOLD_PIN, threshold);
  // 白線を取得
  for(int i=0;i<ANALOG_NUM;i++) {
    analog[i] = analogRead(ANALOG_PIN[i]);

    if(i==7) Serial.print(" ");
    // Serial.print(analog[i]);
    Serial.print(" ");
  }
  Serial.print("        ");

  for(int i=0;i<LINE_NUM;i++) line[i] = digitalRead(LINE_PIN[i]);
  for(int i=0;i<LINE_NUM;i++){
    if(i<INNER_NUM && (i)%5 == 0 && i!=0) Serial.print(" ");
    if(i==INNER_NUM)                Serial.print("   ");
    if(i==INNER_NUM+5)              Serial.print("  ");
    if(i==INNER_NUM+10)             Serial.print("  ");
    if(i==INNER_NUM+15)             Serial.print("  ");
    // Serial.print(line[i]);
  }
  Serial.print("    ");
  // Serial.print(threshold);
  // Serial.println();

  // delay(25);

  // 白線のベクトルを計算
  // しおからさんのアルゴリズムは複雑で実装がめんどくさい
  // → 反応したセンサのベクトルの平均値を白線のベクトルとする代替アルゴリズムを検証
  /*
  */

  line[0] = 0;
  line[4] = 0;
  line[13] = 0;

  double x = 0.0;
  double y = 0.0;
  uint8_t responded = 0;
  for(int i=0;i<INNER_NUM;i++){
    if(line[i]){
      double sensor_dir = radians(i*360.0/INNER_NUM - 90.0); // rad
      x += cos(sensor_dir);
      y += sin(sensor_dir);
      responded++;
    } 
  }
  x /= responded;
  y /= responded;

  double line_dir = -degrees(atan2(y,x)) - 90;
  if(line_dir > 180) line_dir = line_dir - 360;
  // if(line_dir > 180) line_dir = line_dir - 360;

  Serial.print("x:");
  Serial.print(x);
  Serial.print("\ty:");
  Serial.print(y);
  Serial.print("\tdir:");
  Serial.println(line_dir);
}