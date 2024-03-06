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

  bool gameTimeRemaining(int timeGameStarted, int totalGameTime);
  int getTeamColour(int team);

  void showDistance(int currentDistance, int colour);
  void printGameTimeRemaining(int time);
  void setAllLEDs(int colour);
  void startWiFi(int team);
  void run(int &team);

 private:
  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  int wifiChannel = 10;

  bool log = true;  // Used to turn logging on and off

  int currentDistance;
  int maxDistance = 100;
  int bitingDistance = 50;  //? Set to 40 for actual game

  //! Used for the buffered version of the game (Not implemented)
  // Set the buffer to biting distance + 1 to handle not finding the zombie network
  int timeTillTurned = 1.5 * 1000;
  int buffer[5] = {bitingDistance + 1, bitingDistance + 1, bitingDistance + 1, bitingDistance + 1, bitingDistance + 1};
  int bufferPos = 0;
  int bufferAverage = 0;

  int timeNow = 0;
  int timeCaught = 0;

  enum Team {
    human,
    zombie
  };
};
#endif
