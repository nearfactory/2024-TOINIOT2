#include "ball.hpp"
#include "line.hpp"
#include "motor.hpp"

void setup() {
  Serial.begin(9600);
  Serial.println("IOT2 2024-06-06 AtMega2560 Test");

  ballSetup();
  lineSetup();
  motorSetup();
}

void loop() {
  static int count=0;
  static int processing_start = 0;
  processing_start = millis();

  static short ball[BALL_NUM]{1023};
  static bool line[LINE_NUM]{false};
  
  lineUpdate(line);
  ballUpdate(ball);
  motorDebug();

  Serial.print("processing time:");
  Serial.print(millis()-processing_start);
  Serial.println("(ms)");
  Serial.println("\n--------------------------------\n");
  count++;
  delay(50);
}