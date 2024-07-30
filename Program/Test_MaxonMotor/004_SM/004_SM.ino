// Motordriver Control
// 2024.07.30

/*

PH: Direction
EN: Speed(PWM)

*/
const int pinMotor[4][2] = {{28, 29}, {6, 9}, {4, 5}, {2, 3}}; 

float motorPower[4] = {100, 100, -100, -100};

float theta = 0;

void setup() {
  for(auto pin : pinMotor) {
    pinMode(pin[0], OUTPUT);
    pinMode(pin[1], OUTPUT);
    // PWM-frequency Set to 50kHz
    analogWriteFrequency(pin[1], 50000);
  }

  Serial.begin(115200);
}

void loop() {
  for(int i=0; i<4; i++){
    motorOutput(motorCalc(motorPower[i]), pinMotor[i][0], pinMotor[i][1]);
    motorPower[i] = cos(theta) * 100;
    if(i > 2) motorPower[i] *= -1;
  }

  for(int i=0; i<4; i++) {
    Serial.print(i+1);
    Serial.print(":");
    Serial.print(motorPower[i]);
    Serial.print("\t");
  }
  Serial.println();

  theta += 0.02;

  delay(100);
}

// Calculate Motor-power
int motorCalc(float power){
  power = power > 100 ? 100 : power;
  power = power < -100 ? -100 : power;
  power = power / 100 * 255;
  return (int)power;
}

// Output Motor
void motorOutput(int power, char PH, char EN){
  if(power >= 0){
    digitalWrite(PH, 1);
  }
  else{
    digitalWrite(PH, 0);
    power *= -1;
  }
  analogWrite(EN, power);
}