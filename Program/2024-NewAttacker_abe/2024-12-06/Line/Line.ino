constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;
const     uint8_t LINE_PIN[LINE_NUM] = {
  7,8,9,10,11, 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
};

constexpr uint8_t ANALOG_NUM = 12;
const     uint8_t ANALOG_PIN[ANALOG_NUM] = {
  A11, A10, A2, A3, A1, A9, A8, A6, A0, A7, A5, A4
};

constexpr uint8_t THRESHOLD_PIN = 2;

bool line[LINE_NUM];
int analog[ANALOG_NUM];

uint32_t max = 0;
uint32_t avr = 0;
uint32_t count = 0;

uint8_t threshold = 630/4;

void setup(){
  Serial.begin(115200);
  // Serial.println("TOINIOT2 Line 2024-10-12");

  for(auto p:LINE_PIN) pinMode(p, INPUT_PULLUP);
  for(auto p:ANALOG_PIN) pinMode(p, INPUT_PULLUP);

  auto begin_ms = millis();
  while(millis()-begin_ms < 5000){
    for(int i=0;i<ANALOG_NUM;i++){

      if(i != 4){
        int s = analogRead(ANALOG_PIN[i]);
        if(s > max){
          max = s;
        }
        else{
          avr += s;
          count++;
        }
      }
      
    }
    delay(50);
  }

  avr = avr / count;
  threshold = (avr*4+max)/5/4;



  // Serial.println();
  // Serial.println(threshold*4);
  // delay(1000);



  pinMode(THRESHOLD_PIN, OUTPUT);
}

// U1
// 常に1 U5 U14 U24 U38-40

// 先頭3ビット
// 30/5 = 6

// 0b0001 1111  0

// 0b001x xxxx  1
// 0b010x xxxx  2
// 0b011x xxxx  3
// 0b100x xxxx  4
// 0b101x xxxx  5
// 0b110x xxxx  6

// 0b0000 0000  7

char send_str[8]{};
constexpr uint8_t DATA_SIZE = 5;

void loop(){
  analogWrite(THRESHOLD_PIN, threshold);

  // 白線を取得
  for(int i=0;i<LINE_NUM;i++){
    line[i] = digitalRead(LINE_PIN[i]);
  }

  // 送信する文字列の準備
  for(int i=0;i<7;i++){
    send_str[i] = i << DATA_SIZE;
  }
  
  // 圧縮
  for(int i=0;i<INNER_NUM;i++){
    send_str[i/DATA_SIZE + 1] |= line[i] << (DATA_SIZE - i%DATA_SIZE - 1);
  }

  line[37] = 0;
  line[38] = 0;
  line[39] = 0;

  send_str[6] |= (line[INNER_NUM]   |line[INNER_NUM+1] |line[INNER_NUM+2] |line[INNER_NUM+3] |line[INNER_NUM+4])  << 3;  // 前
  send_str[6] |= (line[INNER_NUM+5] |line[INNER_NUM+6] |line[INNER_NUM+7] |line[INNER_NUM+8] |line[INNER_NUM+9])  << 2;  // 左
  send_str[6] |= (line[INNER_NUM+10]|line[INNER_NUM+11]|line[INNER_NUM+12]|line[INNER_NUM+13]|line[INNER_NUM+14]) << 1;  // 右
  send_str[6] |= (line[INNER_NUM+15]|line[INNER_NUM+16]|line[INNER_NUM+17]|line[INNER_NUM+18]|line[INNER_NUM+19]) << 0;  // 後

  send_str[0] = 0b00011111;
  send_str[7] = '\0';

  // 送信
  Serial.print(send_str);


  // テスト
  // for(int i=0;i<ANALOG_NUM;i++){
  //   Serial.print(analogRead(ANALOG_PIN[i]));
  //   Serial.print("\t");
  // }
  // Serial.println();

  delay(5);
}


/*
constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;
const     uint8_t LINE_PIN[LINE_NUM] = {
  7,8,9,10,11, 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
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
    Serial.print(analog[i]);
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

    // if(i==7-1 || i==5-1 || i==14-1 || i== 38-1 || i==39-1 || i==40-1)
    //   Serial.print("_");
    // else
      Serial.print(line[i]);
  }
  Serial.print("    ");
  Serial.print(threshold*4);
  // Serial.println();

  // delay(25);

  // 白線のベクトルを計算
  // しおからさんのアルゴリズムは複雑で実装がめんどくさい
  // → 反応したセンサのベクトルの平均値を白線のベクトルとする代替アルゴリズムを検証

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

  Serial.print("   x:");
  Serial.print(x);
  Serial.print("  y:");
  Serial.print(y);
  Serial.print("  dir:");
  Serial.println(line_dir);
}

// センサの値がズレている: 1(+200)
// 常に1: 5 14 38 39 40
// 常に0: 7 
*/