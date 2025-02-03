constexpr int KICKER_SPAN_MS = 2500;  // 射出タイミング間の待ち時間(ms)
constexpr int KICKER_THRESHOLD = 400; // キッカー用照度センサのしきい値

long kickerStart = 0; // 射出タイミング間計測用

void setup() {
  pinMode(D3, OUTPUT);

  Serial.begin(115200);
  Serial.println("Kicker Test Program 2025-02-03");
}

void loop() {
  Serial.println(analogRead(A1));

  if(analogRead(A1) < KICKER_THRESHOLD){
    // ボールを保持しているなら
    Serial.println("Holding");
    if(millis() - kickerStart > KICKER_SPAN_MS){
      // 前回の射出から2500ms以上経過しているなら
      digitalWrite(D3, 1);
      kickerStart = millis();
      delay(50);
      digitalWrite(D3, 0);
    }
  }
  else{
    // ボールを保持していないなら
    Serial.println("Waiting");
  }
  delay(50);
}
