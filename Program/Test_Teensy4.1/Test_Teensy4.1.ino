#include <ADC.h>

ADC adc{};

void setup() {
  Serial.begin(9600);
  pinMode(A2, INPUT);
  //adc.setAveraging(8);
}

void loop() {
  int raw=analogRead(A2);
  Serial.println(raw);
  delay(50);
}
