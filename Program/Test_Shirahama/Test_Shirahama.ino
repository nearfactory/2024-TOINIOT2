// motor
constexpr uint8_t MOTOR_NUM = 4;
constexpr uint8_t MOTOR_PWM_TYPE = 2;
const uint8_t MOTOR_PIN[MOTOR_NUM][MOTOR_PWM_TYPE]{
  {29, 28},
  {9, 6},
  {5, 4},
  {3, 2}
};
enum MOTOR : uint8_t{
  EN = 0,
  PH
};

// osushi
constexpr uint8_t OSUSHI_PIN = 32;

// ball
constexpr int BALL_NUM = 16;
const uint8_t BALL_PIN[BALL_NUM] = {
  // 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,  26, 27, 38, 39, 40, 41
  A0, A1, A2, A3, A4, A5, A6, A7, A8, A9,  A12, A13, A14, A15, A16, A17
};

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 Shirahama Test 2024-08-02");

  // motor
  for(int i=0;i<MOTOR_NUM;i++){
    pinMode(MOTOR_PIN[i][MOTOR::EN], OUTPUT);
    pinMode(MOTOR_PIN[i][MOTOR::PH], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][MOTOR::EN], 40000);
  }

  // osushi
  pinMode(OSUSHI_PIN, INPUT);

  // ball
  for(auto p:BALL_PIN) pinMode(p, INPUT);
}

void loop() {
  // motor
  uint8_t power = 128;
  Serial.print("motor : ");
  for(int i=0;i<MOTOR_NUM;i++){
    analogWrite(MOTOR_PIN[i][MOTOR::EN], power);
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], 1);
    Serial.printf("%04d ", power);
  }
  Serial.println();

  // osushi
  static auto previous_kick_ms = millis();
  if( millis()-previous_kick_ms > 5000){
    digitalWrite(OSUSHI_PIN, 1);
    // delay(10);
    digitalWrite(OSUSHI_PIN, 0);

    previous_kick_ms = millis();
    Serial.println("kick!");
  }else{
    digitalWrite(OSUSHI_PIN, 0);
  }

  // ball
  Serial.print("ball  : ");
  for(auto p:BALL_PIN){
    Serial.printf("%04d ", analogRead(p));
  }
  Serial.println();

  delay(50);
}
