#ifndef LOBBIES_H
#define LOBBIES_H

#include "./leds/leds.h"
#include "FastLED.h"

class Lobbies : public LEDs {
 public:
  Lobbies();
  Lobbies(int totalLEDs, CRGB *currentLED);  // Constructor

  void preGameLobby(int colour);
  void countDownAnimation(int colour);
  void endGameLobby(int colour);
  void reset();

  bool lobbyCountdownFinished(int timeLobbyCountdownStarted, int lobbyCountdownTime);

 private:
  void setAllLEDs(int colour);

  void printTimeRemaining(int time);

  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  LEDs leds;

  int currentMillis;
  int lastMillis;

  bool flipFlop = false;

  int brightness;

  int preGameLobbyPos = totalLEDs - 1;   // leds are wired up counter clockwise
  int postGameLobbyPos = totalLEDs - 1;  // leds are wired up counter clockwise
};
#endif
