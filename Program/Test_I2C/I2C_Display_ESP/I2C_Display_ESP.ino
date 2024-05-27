#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

using uint8_t = unsigned char;

constexpr uint8_t DISPLAY_ADDR = 0x03c;
constexpr int DISPLAY_W = 128;
constexpr int DISPLAY_H = 64;

constexpr int OLED_RESET = -1;

Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire, OLED_RESET);

void setup() {
  Wire.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)){
    while(true);
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 10);
  display.print("TAMANEGI");
  display.setCursor(10, 30);
  display.print("I2C0 0x3c");

  display.display();
}

void loop() {

}
