void setup() {
  Serial.begin(115200);

  Serial1.setPins(6, 7);
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


void loop() {
  if(Serial1.available()){

  }
  Serial1.print

  delay(1);
}