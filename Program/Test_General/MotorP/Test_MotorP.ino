constexpr uint8_t MOTOR_EN = 9;
constexpr uint8_t MOTOR_PH = 6;

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_PH, OUTPUT);
  analogWriteFrequency(MOTOR_EN, 40000);
}

double motor_now = 0;
double motor_goal = 0;
double prev_goal = 0;

int p_step = 10;
int p_count = 0;
double p_val = 0;

int btn1_prev = 0;
int btn2_prev = 0;

void loop() {
  if(btn1_prev && !digitalRead(36))
    motor_goal += 50;
  if(btn2_prev && !digitalRead(37))
    motor_goal -= 50;

  motor_goal = motor_goal > -255 ? motor_goal : -255;
  motor_goal = motor_goal < 255  ? motor_goal :  255;

  btn1_prev = digitalRead(36);
  btn2_prev = digitalRead(37);
  
  myP(motor_goal);
  prev_goal = motor_goal;

  Serial.printf("min:%lf  max:%lf  ", -255.0, 255.0);
  Serial.printf("motor_goal:%lf  p_val:%lf  motor_now:", motor_goal, p_val);
  Serial.println(motor_now);
  delay(20);
}

void myP(double power){
  if(prev_goal == motor_goal  &&  p_count < p_step){
    Serial.println("A");
    motor_now += p_val;
    p_count++;
  }else if(prev_goal != motor_goal){
    Serial.println("B");
    p_val = (motor_goal - motor_now) / p_step;
    motor_now += p_val;
    p_count = 1;
  }else{
    Serial.println("C");
  }

  return;
}