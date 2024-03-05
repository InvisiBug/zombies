#ifndef GAMEENGINE_H
#define GAMEENGINE_H
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

class GameEngine {
 public:
  int gameState = lobby;

  GameEngine(int totalLEDs, CRGB *currentLED);  // Constructor

  void run();
  void begin();
  void setGameState(int gameState);
  void startLobbyCountdown();

  void topButtonClicked();
  void bottomButtonClicked();

 private:
  int getTeamColour();
  void reset();
  void startWiFi(int team);

  bool gameTimeRemaining(int timeGameStarted);
  void printGameTimeRemaining(int timeGameStarted);
  void showTimeLeft(int timeGameStarted);

  CRGB *currentLED;  // LED strip object
  Lobbies lobbies;
  int totalLEDs;

  int timeLobbyCountdownStarted;  // Used to keep track time left in lobby
  int timeGameStarted;            // Used to keep track of time left in game

  int wifiChannel = 10;

  int totalGameTime = 3 * 1000;  // In seconds

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
