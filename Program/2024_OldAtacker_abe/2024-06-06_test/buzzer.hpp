#pragma once
#ifndef BUZZER_NOUSE

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

inline void buzzerSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
  analogWrite(BUZZER_PIN, SOUND_PITCH[0]);
  delay(100);
  analogWrite(BUZZER_PIN, 0);
};


inline void buzzer(float pitch) {
  if(pitch!=0.0f)
    analogWrite(BUZZER_PIN, pitch);
  return;
}

inline void buzzer(uint8_t pitch) {
  if(pitch!=0.0f)
    analogWrite(BUZZER_PIN, SOUND_PITCH[pitch]);
  return;
}

inline void buzzer(void) {
  analogWrite(BUZZER_PIN, SOUND_PITCH[0]);
  return;
}

#endif