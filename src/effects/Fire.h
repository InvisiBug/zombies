#ifndef FIRE_H
#define FIRE_H
#include "FastLED.h"

class Fire {
 public:
  Fire(int totalLEDs, CRGB *currentLED);

  void run(int Cooling, int Sparkling, int SpeedDelay, bool direction);
  void setPixelHeatColor(int Pixel, uint8_t temperature, bool direction);

  void setPixel(bool direction, int Pixel, uint8_t red, uint8_t green, uint8_t blue);
  int setColour(int r, int g, int b);

 private:
  CRGB *currentLED;  // LED strip object
  int totalLEDs;
};
#endif