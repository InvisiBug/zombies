#ifndef GAME_H
#define GAME_H

#include "FastLED.h"

#define humanColour 0x0000ff
#define zombieColour 0x008800
#define randomColour 0x352465
#define timeColour 0xacacac

class Game {
 public:
  Game();
  Game(int totalLEDs, CRGB *currentLED);  // Constructor
  void begin();

  bool gameTimeRemaining(int timeGameStarted, int totalGameTime);
  void printGameTimeRemaining(int time);
  void reset();
  void run(int &team);

  void startWiFi(int team);

  int getTeamColour(int team);
  void showDistance(int currentDistance, int colour);

 private:
  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  void setAllLEDs(int colour);

  int wifiChannel = 10;
  // int gameState = lobby;

  int currentDistance;
  int maxDistance = 100;
  int bitingDistance = 10;  //? Set to 40 for actual game
  int timeTillTurned = 1.5 * 1000;

  int currentMillis;
  int lastMillis;

  enum Team {
    human,
    zombie
  };
};
#endif
