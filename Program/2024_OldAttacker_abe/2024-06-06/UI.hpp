#pragma once
#ifndef UI_NOUSE

constexpr int BTN_NUM = 4;
const uint8_t BTN_PIN[BTN_NUM] = { 7, 8, 9, 10 };
constexpr int LED_NUM = 3;
const uint8_t LED_PIN[LED_NUM] = { 13, 14, 15 };

constexpr int TONE_NUM = 24;
constexpr uint8_t BUZZER_PIN = 6;
const float SOUND_PITCH[TONE_NUM] = {
  261.626,  // 0: C
  277.183,  // 1: C#
  293.665,  // 2: D
  311.327,  // 3: D#
  329.628,  // 4: E
  349.228,  // 5: F
  369.994,  // 6: F#
  391.995,  // 7: G
  415.305,  // 8: G#
  440.000,  // 9: A
  466.164,  // 10: A#
  493.883,  // 11: B
  SOUND_PITCH[0] * 2,
  SOUND_PITCH[1] * 2,
  SOUND_PITCH[2] * 2,
  SOUND_PITCH[3] * 2,
  SOUND_PITCH[4] * 2,
  SOUND_PITCH[5] * 2,
  SOUND_PITCH[6] * 2,
  SOUND_PITCH[7] * 2,
  SOUND_PITCH[8] * 2,
  SOUND_PITCH[9] * 2,
  SOUND_PITCH[10] * 2,
  SOUND_PITCH[11] * 2,
};


// LED
inline void LEDSetup(){
  for(auto p:LED_PIN) pinMode(p, OUTPUT);
  return;
}

inline void setLED(bool* led){
  for(int i=0;i<LED_NUM;i++){
    digitalWrite(LED_PIN[i], led[i]);
  }
  return;
}

// button
inline void buttonSetup(){
  for(auto p:BTN_PIN) pinMode(p, INPUT);
  return;
}

inline void buttonUpdate(bool *button){
  for(int i=0;i<BTN_NUM;i++){
    button[i] = digitalRead(BTN_PIN[i]);
  }
  return;
}

// buzzer
inline void buzzerSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
  analogWrite(BUZZER_PIN, SOUND_PITCH[0]);
  delay(100);
  analogWrite(BUZZER_PIN, 0);
};

inline void setBuzzer(float pitch) {
  analogWrite(BUZZER_PIN, pitch);
  return;
}

inline void setBuzzer(uint8_t pitch) {
  analogWrite(BUZZER_PIN, SOUND_PITCH[pitch]);
  return;
}

inline void setBuzzer(void) {
  analogWrite(BUZZER_PIN, SOUND_PITCH[0]);
  return;
}

#endif