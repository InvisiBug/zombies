#ifndef LOBBIES_H
#define LOBBIES_H
#include "FastLED.h"

class Lobbies {
 public:
  Lobbies(int totalLEDs, CRGB *currentLED);  // Constructor

  void run();
  void begin();

 private:
  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  int wifiChannel = 10;

  int team = human;
  int gameState = lobby;

  enum Gamestate {
    lobby,
    countdown,
    runGame,
    postGame
  };

  enum Team {
    human,
    zombie
  };
};
#endif
