// Arduino Include
#include <Arduino.h>

// Header Include
#include "tone.h"

// Other Include
#include "pin.h"

// Constant
const float soundPitch[24] = {
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
  soundPitch[0]*2, 
  soundPitch[1]*2, 
  soundPitch[2]*2, 
  soundPitch[3]*2, 
  soundPitch[4]*2, 
  soundPitch[5]*2, 
  soundPitch[6]*2, 
  soundPitch[7]*2, 
  soundPitch[8]*2, 
  soundPitch[9]*2, 
  soundPitch[10]*2, 
  soundPitch[11]*2, 
};

// Function