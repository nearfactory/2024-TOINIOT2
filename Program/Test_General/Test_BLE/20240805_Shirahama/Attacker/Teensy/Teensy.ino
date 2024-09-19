#include <string>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

Adafruit_SSD1306 display(128,64,&Wire2,-1);

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 BLE-UART Test 2024-08-05");

  Wire2.begin();
  while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c));
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(8, 8);
  display.println("receive:");
  display.display();

  Serial2.begin(9600);
}

void loop() {
  static uint32_t c = 0;
  Serial2.println(std::to_string(c).c_str());
  c++;
  if(Serial2.available()){
    std::string receive_str{};
    // receive_str = "";
    while(Serial2.available()){
      receive_str += static_cast<char>(Serial2.read());
    }
    display.clearDisplay();
    display.setCursor(8,8);
    display.println("receive:");
    display.print(receive_str.c_str());
    display.display();
  }


  delay(50);
}
