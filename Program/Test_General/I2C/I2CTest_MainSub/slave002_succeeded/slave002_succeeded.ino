// Wire Slave Sender
// This example code is in the public domain.

#include <string>
#include <Wire.h>

int led = LED_BUILTIN;

void setup()
{
  pinMode(led, OUTPUT);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop()
{
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  static int count=0;
  count++;
  digitalWrite(led, HIGH);  // briefly flash the LED
  Wire.write(std::to_string(count).c_str());     // respond with message of 6 bytes
                            // as expected by master
  digitalWrite(led, LOW);
}