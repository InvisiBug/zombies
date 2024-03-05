#include "leds.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Arduino.h"
#include "FastLED.h"
#include "Streaming.h"
using namespace std;

#define ledPin 14  // D5

#define data 4   // D2
#define clock 5  // D1

// need to inherit the led class and use it here as well as anywhere else using leds

LEDs::LEDs() {
}

LEDs::LEDs(int totalLEDs) {
  Serial << "here" << endl;

  CRGB currentLED[totalLEDs];
  this->currentLED = currentLED;

  FastLED.addLeds<APA102, data, clock, BGR, DATA_RATE_MHZ(16)>(currentLED, totalLEDs);  // * May not need to specify the data rate
  // FastLED.addLeds<APA102, data, clock, BGR, DATA_RATE_MHZ(16)>(currentLED, totalLEDs);  // * May not need to specify the data rate

  FastLED.setBrightness(255);
}

LEDs::LEDs(int totalLEDs, CRGB *currentLED) {
  /*
   * Couldnt figure out how to initilize the leds in this classes, passed in instead
   */

  this->totalLEDs = totalLEDs;

  // CRGB currentLED[totalLEDs];
  this->currentLED = currentLED;
  // this->currentLED = CRGB currentLED[totalLEDs];

  ///////
  // LEDs
  // FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  // FastLED.addLeds<APA102, data, clock, BGR, DATA_RATE_MHZ(24)>(currentLED, totalLEDs);  // * May not need to specify the data rate
  // FastLED.addLeds<APA102, data, clock, BGR, DATA_RATE_MHZ(16)>(currentLED, totalLEDs);  // * May not need to specify the data rate

  // FastLED.setBrightness(LEDBrightness * 2.55);
  // FastLED.setCorrection(0xFFB0F0);
  // FastLED.setDither( 1 );
}

void LEDs::begin() {
  Serial << "On" << endl;
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = 0xff0000;
  }
  FastLED.show();
  delay(500);

  Serial << "Off" << endl;
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
  delay(500);
}

void LEDs::reset() {
}

//////////////////////////////////////////////////////////////////////////////
//
//  ######
//  #     # #    # #    #
//  #     # #    # ##   #
//  ######  #    # # #  #
//  #   #   #    # #  # #
//  #    #  #    # #   ##
//  #     #  ####  #    #
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// ###
//  #  #    # ##### ###### #####  #    #   ##   #
//  #  ##   #   #   #      #    # #    #  #  #  #
//  #  # #  #   #   #####  #    # #    # #    # #
//  #  #  # #   #   #      #####  #    # ###### #
//  #  #   ##   #   #      #   #   #  #  #    # #
// ### #    #   #   ###### #    #   ##   #    # ######
//
//////////////////////////////////////////////////////////////////////////////
