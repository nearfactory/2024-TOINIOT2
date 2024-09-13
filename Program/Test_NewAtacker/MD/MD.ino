// MD Test 2024-08-01

constexpr int SW_TAC_NUM = 5;
const uint8_t SW_TAC_PIN[SW_TAC_NUM] = { 36,37,31,30,32 };
enum SW : uint8_t{
  TAC_1 = 0,
  TAC_2,
  TAC_3,
  TAC_4,
  TAC_5
};

constexpr int MOTOR_NUM = 4;
constexpr int MOTOR_PWM_TYPE = 2;
const uint8_t MOTOR_PIN[MOTOR_NUM][MOTOR_PWM_TYPE]{
  {29, 28},
  {9, 6},
  {5,4},
  {3,2}
};
const uint8_t DEFAULT_MOTOR_DIR[MOTOR_NUM] = {
  1,1,1,1
};
enum MOTOR : uint8_t{
  LF = 0,
  LB,
  RB,
  RF,
  EN = 0,
  PH,
};


void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 MD Test 2024-08-01");
  
  for(auto p:SW_TAC_PIN) pinMode(p, INPUT);

  for(int i=0;i<MOTOR_PWM_TYPE;i++){
    pinMode(MOTOR_PIN[i][0], OUTPUT);
    pinMode(MOTOR_PIN[i][1], OUTPUT);
    analogWriteFrequency(MOTOR_PIN[i][0], 400000);
    // analogWriteFrequency(MOTOR_PIN[i])
  }

  
}

void loop() {
  /*
  */


  // sw-tac
  bool sw_tac[SW_TAC_NUM]={false};
  for(int i=0;i<SW_TAC_NUM;i++) sw_tac[i] = digitalRead(SW_TAC_PIN[i]);

  static uint32_t count = 0;

  short power = sin(count/64.0) * 127;
  // static short power = 128;
  // if(sw_tac[0]) power++;
  // if(sw_tac[1]) power--;

  power = power<-255 ? -255 : power;
  power = power>255 ? 255 : power;

  uint8_t motor_power = abs(power);
  uint8_t dir = power<0 ? 1 : 0;

  // analogWrite()

  for(int i=0;i<MOTOR_NUM;i++){
    analogWrite(MOTOR_PIN[i][MOTOR::EN], 127);
    digitalWrite(MOTOR_PIN[i][MOTOR::PH], 0);
  }

  Serial.printf("\n\npower:%d\nmotor_power:%d\ndir:%d\n", power, motor_power, dir);

  count++;
  delay(50);
}
