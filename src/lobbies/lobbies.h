#ifndef LOBBIES_H
#define LOBBIES_H

#include "./leds/leds.h"
#include "FastLED.h"

class Lobbies : public LEDs {
 public:
  Lobbies();
  Lobbies(int totalLEDs, CRGB *currentLED);  // Constructor
  void begin();

  void preGameLobby(int colour);
  void endGameLobby(int colour);
  void countDownAnimation(int colour);
  void reset();

  bool lobbyCountdownFinished(int timeLobbyCountdownStarted);

 private:
  void setAllLEDs(int colour);

  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  LEDs leds;

  int wifiChannel = 10;

  int team = human;
  // int gameState = lobby;

  int currentMillis;
  int lastMillis;

  bool flipFlop = false;

  int countDownDuration = (5 * 1000);

  int preGameLobbyPos = totalLEDs - 1;   // leds are wired up counter clockwise
  int postGameLobbyPos = totalLEDs - 1;  // leds are wired up counter clockwise

  enum Team {
    human,
    zombie
  };
};
#endif
