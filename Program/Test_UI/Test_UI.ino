// Teensy4.1
// UI Test


// UI(左側) - LABEL - Teensy

// 10 - BUZZER - 0

// 9 - SCL - 24
// 8 - SDA - 25

// 7 - SW-TAC-5 - 1 (KickerSignal)
// 6 - SW-TAC-4 - 2
// 5 - SW-TAC-3 - 3
// 4 - SW-TAC-2 - 4
// 3 - SW-TAC-1 - 5
// 2 - GND
// 1 - +3.3V

// UI(右側)


#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

constexpr int SW_TAC_NUM = 5;
const uint8_t SW_TAC_PIN[SW_TAC_NUM] = { 36,37,31,30,32 };
enum SW : uint8_t{
  TAC_1 = 0,
  TAC_2,
  TAC_3,
  TAC_4,
  TAC_5
};

constexpr uint8_t BUZZER_PIN = 33;

constexpr uint8_t DISPLAY_W = 128;
constexpr uint8_t DISPLAY_H = 64;
constexpr uint8_t OLED_RESET = -1;
Adafruit_SSD1306 display(DISPLAY_W, DISPLAY_H, &Wire2, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial.println("TOINIOT2 UI Test 2024-07-31");

  for(auto p:SW_TAC_PIN) pinMode(p, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("pinMode");

  Wire2.begin();
  Serial.println("I2C");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("Display error!");
    while(true);
  }

  display.clearDisplay();
  // display.setTextSize(1);
  display.setCursor(8, 8);
  display.println("TOINIOT2 UI Test 2024-07-31");
  display.display();

  Serial.println("display");

  Serial.println("setup()");
  delay(256);
}

void loop() {
  display.clearDisplay();
  // display.setTextSize(1);
  display.setTextColor(WHITE);
  auto begin_ms = millis();


  // sw-tac
  bool sw_tac[SW_TAC_NUM]={false};
  for(int i=0;i<SW_TAC_NUM;i++) sw_tac[i] = digitalRead(SW_TAC_PIN[i]);


  // buzzer (1,2)
<<<<<<< Updated upstream
  static float buzzer = 0.0f;
=======
<<<<<<< Updated upstream
  static float buzzer = 440.0f;
=======
  static float buzzer = 0.0f;
>>>>>>> Stashed changes
>>>>>>> Stashed changes
  if(sw_tac[SW::TAC_1]) buzzer += 4.0f;
  if(sw_tac[SW::TAC_2]) buzzer -= 4.0f;
  buzzer = buzzer<0.0f ? 0.0f : buzzer;

  // buzzer = buzzer>255.0f ? 255.0f : buzzer;
  // analogWrite(BUZZER_PIN, (int)buzzer);
  tone(BUZZER_PIN, buzzer);
  
  display.setCursor(8, 8);
  display.print("bz+");
  display.setCursor(8, 48);
  display.print("bz-");


  // val (3,4)
  static int8_t val;
  val += sw_tac[SW::TAC_3];
  val -= sw_tac[SW::TAC_4];

  display.setCursor(8, 28);
  display.printf("val:%d", val);

  display.setCursor(88, 8);
  display.print("val+");
  display.setCursor(88, 28);
  display.print("val-");


  // kicker (5)
  display.setCursor(88, 48);
  // if(sw_tac[SW::TAC_5]) display.print("kick");
  display.print("kick");
  // display.invertDisplay(sw_tac[SW::TAC_1]);
  display.invertDisplay(sw_tac[SW::TAC_5]);
  // display.invertDisplay(true);
  // digitalWrite(SW_TAC_PIN[4], 1);
  // digitalWrite(SW_TAC_PIN[4], 0);
  // delay(1000);


  // ----
  display.display();
  Serial.print("sw-tac:");
  for(auto s:sw_tac) Serial.print(s);
  Serial.printf("\nval:%d", val);
  Serial.printf("\nbuzzer:%f", buzzer);
  Serial.printf("\nprocess:%d(ms)\n\n\n", millis()-begin_ms );

  // delay(25);
}