#define LED1_PIN (A1)  // 白 普通    OSWT3166B
#define LED2_PIN (A2)  // 赤 低輝度  OSR5JA3Z74A
#define LED3_PIN (A3)  // 赤 高輝度  OS5RKA3131A

#define SAMPLE 50

short led1[SAMPLE];
short led2[SAMPLE];
short led3[SAMPLE];

int count = 0;

void setup() {
  pinMode(LED1_PIN, INPUT);
  pinMode(LED2_PIN, INPUT);
  pinMode(LED3_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  led1[count] = analogRead(LED1_PIN);
  led2[count] = analogRead(LED2_PIN);
  led3[count] = analogRead(LED3_PIN);

  uint32_t led1_avr = 0, led2_avr = 0, led3_avr = 0;
  for(auto l:led1) led1_avr += l;
  for(auto l:led2) led2_avr += l;
  for(auto l:led3) led3_avr += l;
  led1_avr /= SAMPLE;
  led2_avr /= SAMPLE;
  led3_avr /= SAMPLE;

  Serial.print("count:");
  Serial.print(count);
  Serial.print(" min:0 max:1023");

  Serial.print(" LED1:");
  Serial.print(led1[count]);
  Serial.print(" LED2:");
  Serial.print(led2[count]);
  Serial.print(" LED3:");
  Serial.print(led3[count]);

  Serial.print(" LED1_avr:");
  Serial.print(led1_avr);
  Serial.print(" LED2_avr:");
  Serial.print(led2_avr);
  Serial.print(" LED3_avr:");
  Serial.println(led3_avr);

  count++;
  count %= SAMPLE;
  delay(50);
}
