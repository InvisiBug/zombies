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

  void showDistance(int currentDistance, int colour);
  void printGameTimeRemaining(int time);
  void startWiFi(int team);
  void run(int &team);

  bool gameTimeRemaining(int timeGameStarted, int totalGameTime);

  int getTeamColour(int team);

 private:
  CRGB *currentLED;  // LED strip object
  int totalLEDs;

  void setAllLEDs(int colour);

  int wifiChannel = 10;

  int currentDistance;
  int maxDistance = 100;
  int bitingDistance = 10;  //? Set to 40 for actual game
  int timeTillTurned = 1.5 * 1000;

  //! Used for the buffered version of the game (Not implemented)
  // Set the buffer to biting distance + 1 to handle not finding the zombie network
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
