constexpr uint8_t BALL01K_PIN  = A0;
constexpr uint8_t BALL02K_PIN  = A1;
constexpr uint8_t VOLUME_PIN   = A2;

constexpr uint8_t KICKER_PIN   = D3;
constexpr uint8_t PH_PIN       = 4;
constexpr uint8_t EN_PIN       = 5;
constexpr uint8_t NEOPIXEL_PIN = 10;




void setup() {
  Serial.begin(115200);

  pinMode(BALL01K_PIN, INPUT);
  pinMode(BALL02K_PIN, INPUT);
  pinMode(VOLUME_PIN, INPUT);

  pinMode(KICKER_PIN, OUTPUT);
  pinMode(PH_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(NEOPIXEL_PIN, OUTPUT);

  Serial1.setPins(D7, D6);
  Serial1.begin(115200);
}


//D0 R ball ->
// 1 L ball ->
// 2 volume ->
// 3 kicker <-
// 4 dribbler ph <-
// 5 dribbler en <-
// 6 tx ->
// 7 rx <-
// 10 neopixel <-

// <(bit)>
// <-(rx)  <kicker(1) dribbler(6) dammy(1)> <neopixel(id:8, rgb:24)>  <null(8)>
// ->(tx)  <b1(7) dammy(1)> <b2(7) dammy(1)> <volume(7) dammy(1)>     <null(8)>


bool kicker = 0;
uint32_t kicker_time = 0;
int dribbler = 0;


void loop() {

  /*
  // 送信
  char send[4] = "";
  send[0] = (analogRead(BALL01K_PIN) >> 4 + 1) & 0b11111111;
  send[1] = (analogRead(BALL02K_PIN) >> 4 + 1) & 0b11111111;
  send[2] = (analogRead(VOLUME_PIN)  >> 4 + 1) & 0b11111111;
  send[3] = '\0';
  Serial1.print(send);

  
  // 受信
  if(Serial1.available()){
    // 最初の1byteを読み出す
    char c = Serial1.read();
    kicker   = (c >> 7) & 0b00000001;
    dribbler = (c >> 1) & 0b00111111;
    while(Serial1.available())
  }

  delay(1);
  */

  Serial1.print("min:0 max:1023 ");
  Serial1.printf("b1:%d\tb2:%d\tvol:%d\n",
    analogRead(BALL01K_PIN),
    analogRead(BALL02K_PIN),
    analogRead(VOLUME_PIN)
  );
  // Serial1.println("fujiki");
  delay(50);

  digitalWrite(KICKER_PIN, HIGH);
  delay(50);
  digitalWrite(KICKER_PIN, LOW);
  delay(5000);
}