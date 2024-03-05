#ifndef LEDS_H
#define LEDS_H

#include "FastLED.h"

class LEDs {
 public:
  LEDs();
  LEDs(int totalLEDs);
  LEDs(int totalLEDs, CRGB *currentLED);  // Constructor
  void begin();
  void reset();

 private:
  // void setAllLEDs(int colour);

  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  int LEDBrightness = 10;

  enum Team {
    human,
    zombie
  };
};
#endif
