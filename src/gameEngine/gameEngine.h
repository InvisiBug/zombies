#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "./lobbies/lobbies.h"
#include "FastLED.h"

// * Calling a class from within a class
// https://forum.arduino.cc/t/solved-creating-instance-of-another-class-inside-a-class/571874
// https://stackoverflow.com/questions/19619843/error-in-c-redefinition-of-class-constructor

class GameEngine {
 public:
  GameEngine(int totalLEDs, CRGB *currentLED);  // Constructor

  void run();
  void begin();

 private:
  CRGB *currentLED;  // LED strip object
  Lobbies lobbies;
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
