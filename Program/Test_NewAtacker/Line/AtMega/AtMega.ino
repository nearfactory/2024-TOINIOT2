constexpr uint8_t LINE_NUM = 32;
const     uint8_t LINE_PIN[LINE_NUM] = {
  12,13,14,15,16,17,18,27, 19,20,21,22,23,24,25,26, 53,54,55,56,57,58,59,60, 63,64,65,66,67,68,69,79
};
bool line[LINE_NUM]{false};

constexpr uint8_t ANALOG_NUM = 8;
const     uint8_t ANALOG_PIN[ANALOG_NUM] = {
  97, 96, 95, 94, 93, 92, 91, 90
};

constexpr uint8_t LED_NUM = 3;
const     uint8_t LED_PIN[LED_NUM] = { 35,36,37 };
          bool    LED[LED_NUM] = {false};

constexpr uint8_t THRESHOLD_PIN = 5;
          short   THRESHOLD = 0;

char receive_str[10];

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line Test Atmega 2024-09-04");

  Serial1.begin(115200);
  Serial1.println("AtMega");

  // 通信テスト
  // ********* countの処理をなんとかする
  while(true){
    int count = 0;
    if(Serial1.available()){
      receive_str[count] = Serial.read();
      count++;
    }
    
    bool correct = false;
    char teensy[] = "Teensy";
    for(int i=0;i<7;i++){
      if(receive_str[i] != teensy[i]) break;

      correct = true;
      digitalWrite(LED_PIN[0], HIGH);
    }

    if(correct) break;
  }

  for(auto p:LINE_PIN) pinMode(p, INPUT);
  
  // 8/17 18:30~
  // ラインセンサ しきい値
  auto ms = millis();
  short max = 0;
  short mix = 1023;
  while(millis()-ms<5000){
    for(auto p:LINE_PIN){
      short val = analogRead(p);

      min = min>p ? p : min;
      max = max<p ? p : max;
    } 
    delay(100);
  }
  THRESHOLD = (min+max)/2;
  analogWrite(THRESHOLD_PIN, THRESHOLD);

  digitalWrite(LED_PIN[1], HIGH);
}

void loop() {
  while(!Serial1.available());

  while(Serial1.available()){
    if(Serial1.read()=='T') break;
  }

  char send_str[7]={
    0b10101010, 0, 0, 0, 0, 0b01010101, \0
  };
  for(int i=0;i<LINE_NUM;i++){
    send_str[1+i/8] = send_str[1+i/8] | digitalRead(LINE_PIN[i]) << (i%8); 
  }

  Serial1.println(send_str);
}
