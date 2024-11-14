constexpr uint8_t LINE_NUM = 46;
constexpr uint8_t INNER_NUM = 26;

bool line[INNER_NUM+4]{0};

constexpr uint8_t DATA_SIZE = 5;
constexpr uint8_t str_size = 8;
char str[str_size] = "";

void setup() {
  Serial.begin(115200);
  Serial.println("TOINIOT2 Line Test Teensy 2024-09-04");

  // pinMode(0,INPUT_PULLDOWN);
  // pinMode(1,INPUT_PULLDOWN);

  // pinMode(0,OUTPUT);
  // pinMode(1,OUTPUT);

  Serial1.begin(9600);
}

void loop() {
  // while(Serial1.available()){
  //   char c = Serial1.read();
  //   for(int i=0;i<8;i++){
  //     Serial.print((c>>(7-i)) & 0b00000001);
  //   }
  //   Serial.print(" ");
  // }
  // Serial.println();

  // 余分な部分を読み飛ばす
  while(Serial1.available()>str_size){
    Serial1.read();
  }
  // 先頭の文字が来るまで読む
  while(Serial1.available()){
    if(Serial1.read() == 0b00011111) break;
  }
  while(Serial1.available()<=str_size-3){int i=0;}

  // // 全部読みだす
  // for(int i=0;i<str_size-1;i++){
  //   str[i] = (char)Serial.read();
  // }

  for(int i=0;i<6;i++){
    char c = Serial1.read();
    for(int j=0;j<5;j++){
      line[i*5+j] = (c>>(4-j)) & 0b00000001;
      Serial.print(line[i*5+j]);
    }
    Serial.print(" ");
  }
  // for(auto l:line) Serial.print(l);

  // while(Serial1.available()){
  //   char c = Serial1.read();
  //   for(int i=0;i<8;i++){
  //     Serial.print((c>>(7-i)) & 0b00000001);
  //   }
  //   Serial.print(" ");
  // }
  Serial.println();


  delay(50);
}