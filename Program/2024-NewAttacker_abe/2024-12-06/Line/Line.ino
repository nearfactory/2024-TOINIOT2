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
uint32_t min = 1023;
uint32_t avr = 0;
uint32_t count = 0;

uint8_t threshold = 630/4;

void setup(){
  Serial.begin(115200);
  // Serial.println("TOINIOT2 Line 2024-10-12");

  for(auto p:LINE_PIN) pinMode(p, INPUT_PULLUP);
  for(auto p:ANALOG_PIN) pinMode(p, INPUT_PULLUP);

  auto begin_ms = millis();
  while(millis()-begin_ms < 3000){
    for(int i=0;i<ANALOG_NUM;i++){

      
    // for(int i=0;i<ANALOG_NUM;i++){
    //   Serial.print(analogRead(ANALOG_PIN[i]));
    //   Serial.print("\t");
    // }
    // Serial.println();


      if(i!=4){
        int s = analogRead(ANALOG_PIN[i]);
        if(s > max){
          max = s;
        }else if(s<min){
          min = s;
        }
        else{
          avr += s;
          count++;
        }
      }

    }
    delay(100);
  }

  avr = avr / count;
  threshold = (avr*4+max-400)/5/4;
  // threshold = (max + 50)/4;



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
  // if(Serial.available()){
  //   char str[10];
  //   int i = 0;
  //   while(Serial.available()){
  //     str[i] = Serial.read();
  //     count++;
  //   }
  //   threshold = atoi(str);
  // }
  // Serial.print(threshold*4);
  // Serial.print("\t");
  if(Serial.available()){
    char c = Serial.read();
    if(c == 'u') threshold += 5;
    if(c == 'd') threshold -= 5;
    // threshold -= 5;
  }
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


  // // テスト
  // Serial.print(threshold);
  // Serial.print("\t");
  // for(auto l:line){
  //   Serial.print(l);
  // }
  // Serial.print("\t");
  // for(int i=0;i<ANALOG_NUM;i++){
  //   Serial.print(analogRead(ANALOG_PIN[i]));
  //   Serial.print("\t");
  // }
  // Serial.println();

  delay(5);
}

// センサの値がズレている: 1(+200)
// 常に1: 5 14 38 39 40
// 常に0: 7 