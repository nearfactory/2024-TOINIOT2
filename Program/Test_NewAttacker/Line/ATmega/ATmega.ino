constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;
const     uint8_t LINE_PIN[LINE_NUM] = {
  7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
};

constexpr uint8_t THRESHOLD_PIN = 2;

bool line[LINE_NUM];

void setup(){
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line 2024-10-12");

  for(auto p:LINE_PIN) pinMode(p, INPUT_PULLUP);
  pinMode(THRESHOLD_PIN, OUTPUT);
}

void loop(){
  analogWrite(THRESHOLD_PIN, 500);
  // 白線を取得
  for(int i=0;i<LINE_NUM;i++) line[i] = digitalRead(LINE_PIN[i]);

  for(auto l:line) Serial.print(l);
  Serial.println();

  delay(50);

  // 白線のベクトルを計算
  // しおからさんのアルゴリズムは複雑で実装がめんどくさい
  // → 反応したセンサのベクトルの平均値を白線のベクトルとする代替アルゴリズムを検証
  /*
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

  Serial.print("x:");
  Serial.print(x);
  Serial.print("\ty:");
  Serial.print(y);
  Serial.print("\tdir:");
  Serial.print(degrees(atan2(y,x)));
  */
}