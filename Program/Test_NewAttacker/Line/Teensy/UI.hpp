#pragma once

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