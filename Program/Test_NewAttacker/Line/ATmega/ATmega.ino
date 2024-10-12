constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;
const     uint8_t LINE_PIN[LINE_NUM] = {
  // 
};

bool line[LINE_NUM];

void setup(){
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line 2024-10-12");

  for(auto p:LINE_PIN) pinMode(p, INPUT);
}

void loop(){
  // 白線を取得
  for(int i=0;i<LINE_NUM;i++) line[i] = digitalRead(LINE_PIN[i]);

  // 白線のベクトルを計算
  // しおからさんのアルゴリズムは複雑で実装がめんどくさい
  // → 反応したセンサのベクトルの平均値を白線のベクトルとする代替アルゴリズムを検証
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
}