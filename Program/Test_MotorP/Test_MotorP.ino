constexpr uint8_t MOTOR_EN = 29;
constexpr uint8_t MOTOR_PH = 28;

void setup() {
  Serial.begin(9500);

  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_PH, OUTPUT);
  analogWriteFrequency(MOTOR_EN, 40000);
}

double motor_now = 0;
double motor_goal = 0;

void loop() {
  /*
  motorP

  motorP(0);
  Serial.println("0");

  motorP(40);
  Serial.println("40");
  */

  for(int i=0;i<400;i++){
    motorFastP(40);
  }
  Serial.println(40);
  delay(400);
  for(int i=0;i<400;i++){
    motorFastP(0);
  }
  Serial.println(0);
  delay(400);

}

void motorRaw(double power){
  analogWrite(MOTOR_EN, power*2.55);
  digitalWrite(MOTOR_PH, power<0);

  return;
}

void motorP(double power){
  static double motor_now = 0;
  static double motor_goal = 0;

  double p_step = 400;
  double motor_buf = 0;
  motor_goal = power;
  
  for(int i=0;i<p_step;i++){
    motor_buf = motor_now + (motor_goal - motor_now) * (i+1) / p_step;
    motorRaw(motor_buf);
    Serial.println(motor_goal-motor_now);
    delay(1);
  }

  motor_now = motor_goal;
  return;
}

void motorFastP(double power){
  double p_step = 400;
  motor_goal = power;
  static double motor_buf = 0;
  motor_now += (motor_goal - motor_now) / p_step;

  motorRaw(motor_now);
  Serial.println(motor_now);

  // motor_now = motor_goal;
  // delay(10);
  return;
}