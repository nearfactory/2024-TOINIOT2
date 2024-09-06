constexpr uint8_t LINE_NUM = 32;
const     uint8_t LINE_PIN[LINE_NUM] = {
  12,13,14,15,16,17,18,27, 19,20,21,22,23,24,25,26, 53,54,55,56,57,58,59,60, 63,64,65,66,67,68,69,79
};

char receive_str[10];

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Line Test Atmega 2024-09-04");

  Serial1.begin(115200);
  Serial1.println("AtMega");

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
    }

    if(correct) break;
  }

  for(auto p:LINE_PIN) pinMode(p, INPUT);
}

void loop() {
  while(!Serial1.available());

  while(Serial1.available()){
    if(Serial1.read()=='T') break;
  }

  char send_str[5];
  for(int i=0;i<LINE_NUM;i++){
    send_str[i/8] = send_str[i/8] | digitalRead(LINE_PIN[i]) << (i%8); 
  }
  send_str[4] = '\0';

  Serial1.println(send_str);
}
