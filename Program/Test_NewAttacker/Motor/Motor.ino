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
  PH,
};

void setup() {
  for(int i=0;i<MOTOR_PWM_TYPE;i++){
    for(int j=0;j<MOTOR_NUM;j++){
      pinMode(MOTOR_PIN[j][i], OUTPUT);
    }
  }

}

void loop() {
  for(int i=0;i<MOTOR_NUM;i++){
    digitalWrite(MOTOR_PIN[i][PH], 1);
    analogWrite(MOTOR_PIN[i][EN], 100);
  }
  delay(50);
}
