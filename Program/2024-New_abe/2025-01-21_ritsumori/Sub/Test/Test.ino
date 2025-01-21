void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

int count = 0;
uint8_t ball01k = 0;
uint8_t ball02k = 0;

constexpr uint8_t QUEUE_SIZE = 4;
uint8_t queue[QUEUE_SIZE][2];
uint8_t queue_id = 0;

void loop() {
  while(Serial2.available()){
    char c = Serial2.read();

    switch(count){
    case 0:
      ball01k = c;
      queue[queue_id][0] = ball01k;
      count++;
      break;
    case 1:
      ball02k = c;
      queue[queue_id][1] = ball02k;
      count++;
      break;
    default:
      count = 0;
      break;
    }
    queue_id = (queue_id+1)%QUEUE_SIZE;
  }

  int sum1 = 0;
  int sum2 = 0;
  for(int i=0;i<QUEUE_SIZE;i++){
    sum1 += queue[i][0];
    sum2 += queue[i][1];
  }
  ball01k = sum1 / QUEUE_SIZE;
  ball02k = sum2 / QUEUE_SIZE;

  Serial.printf("min:0 max:255 th:70 th2:130 ball01k:%d ball02k:%d \n", ball01k, ball02k);
  
  delay(5);
}
