#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "./game/game.h"
#include "./leds/leds.h"
#include "./lobbies/lobbies.h"
#include "FastLED.h"

#define humanColour 0x0000ff
#define zombieColour 0x008800
#define randomColour 0x352465
#define timeColour 0xacacac

// * Calling a class from within a class
// https://forum.arduino.cc/t/solved-creating-instance-of-another-class-inside-a-class/571874
// https://stackoverflow.com/questions/19619843/error-in-c-redefinition-of-class-constructor

// Multiple inheritance
// https://www.geeksforgeeks.org/multiple-inheritance-in-c/

class GameEngine : public Lobbies, public Game {
 public:
  int gameState = lobby;

  GameEngine();                                 // Constructor
  GameEngine(int totalLEDs, CRGB *currentLED);  // Constructor

  void run();
  void begin();
  void setGameState(int gameState);

  void topButtonClicked();
  void bottomButtonClicked();

 private:
  void reset();
  void showTimeLeft(int startTime, int totalTime);

  CRGB *currentLED;  // LED strip object

  //*  Game classes
  Lobbies lobbies;
  LEDs leds;
  Game game;

  int totalLEDs;

  int wifiChannel = 10;

  int timeLobbyCountdownStarted;  // Used to keep track time left in lobby
  int timeGameStarted;            // Used to keep track of time left in game

  // int lobbyCountdownTime = 5 * 1000;       // In seconds
  int lobbyCountdownTime = 2 * 60 * 1000;  // In mins

  // int totalGameTime = 1 * 60 * 1000;   // In mins
  int totalGameTime = 10 * 60 * 1000;  // In mins

  int team = human;

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
