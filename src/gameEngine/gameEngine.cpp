#include "gameEngine.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "./leds/leds.h"
#include "./lobbies/lobbies.h"
#include "Arduino.h"
#include "Streaming.h"
using namespace std;

GameEngine::GameEngine() {}
// GameEngine::GameEngine(int totalLEDs, CRGB *currentLED):lobbies(totalLEDs, currentLED), leds(totalLEDs, currentLED){
GameEngine::GameEngine(int totalLEDs, CRGB *currentLED) : lobbies(totalLEDs, currentLED), game(totalLEDs, currentLED) {
  this->totalLEDs = totalLEDs;
  this->currentLED = currentLED;
}

void GameEngine::begin() {
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
void GameEngine::run() {
  switch (gameState) {
    case lobby:
      /*
       * Sit in the lobby changing team on a button press
       * move on to countdown on button press (below)
       */
      // leds.begin();
      lobbies.preGameLobby(getTeamColour());
      // lobbies.countDownAnimation(getTeamColour());
      break;

    case countdown:
      /*
       * if the timers not finished, run the animation
       * otherwise change the game state to "runGame" and note the time started
       */
      if (!lobbies.lobbyCountdownFinished(timeLobbyCountdownStarted)) {
        lobbies.countDownAnimation(getTeamColour());  // count down animation;

      } else {
        setGameState(runGame);
        timeGameStarted = millis();
        startWiFi(team);  // Only start wifi after the lobby has finished
      }

      break;

    case runGame:
      if (game.gameTimeRemaining(timeGameStarted, totalGameTime)) {
        game.run(team);

      } else {
        WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials
        setGameState(postGame);
      }

      break;

    case postGame:
      lobbies.endGameLobby(getTeamColour());
      break;
  }
}

// TODO: move this to game class

// TODO: Make lobby countdown show time remaining when pressing a button
void GameEngine::showTimeLeft(int timeGameStarted) {
  // int indicatorLevel = map(gameTimeLeft, 0, totalGameTime - 100, 0, totalLEDs);
  int gameTimeLeft = totalGameTime - (millis() - timeGameStarted);

  Serial << "Game time left: " << gameTimeLeft << endl;

  int indicatorLevel = map(gameTimeLeft, 0, totalGameTime - 100, 0, totalLEDs);  // ! Not sure what the -100 is here

  Serial << "Indicator Level: " << indicatorLevel << endl;

  // showDistance(indicatorLevel, timeColour);
  for (int i = 0; i < indicatorLevel; i++) {
    currentLED[i] = timeColour;
  }
  FastLED.show();

  for (int i = totalLEDs; i > indicatorLevel; i--) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
  delay(500);

  for (int i = totalLEDs; i > 0; i--) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}

// TODO: move this to game class

void GameEngine::topButtonClicked() {
  Serial << "Game Engine, Top Button Clicked" << endl;

  switch (gameState) {
    case lobby:
      team = !team;
      break;

    case runGame:
      Serial << "Show game time remaining" << endl;
      showTimeLeft(timeGameStarted);
      break;

    case postGame:
      reset();
      break;
  }
}

void GameEngine::bottomButtonClicked() {
  Serial << "Game Engine, Bottom Button Clicked" << endl;

  switch (gameState) {
    case lobby:
      Serial << "Start lobby countdown" << endl;
      timeLobbyCountdownStarted = millis();
      setGameState(countdown);
      break;

    case runGame:
      Serial << "Show game time remaining" << endl;
      showTimeLeft(timeGameStarted);
      // * LED class show time left, will need to be passed the time remaining
      break;

    case postGame:
      Serial << "Game Engine, Bottom Button Clicked, Post Game" << endl;
      reset();
      break;
  }
}

void GameEngine::setGameState(int gameState) {
  this->gameState = gameState;
}

void GameEngine::reset() {
  setGameState(lobby);
  lobbies.reset();  // need to reset pre and post game lobby counters to (totalLEDs -1)
}

// TODO: Move to game class for now, then to wifi class
void GameEngine::startWiFi(int team) {
  // allOff();
  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  delay(100);  // Added to try and prevent crashing (Remove if not possible)

  /*
    Create the appropriate network for the team, adding the chip id on the end
    can be used for tracking players at a later date
  */
  switch (team) {
    case human:
      Serial << "Player is a human" << endl;

      char humanssid[25];
      snprintf(humanssid, 25, "Human-%06X", ESP.getChipId());
      WiFi.softAP(humanssid, NULL, wifiChannel);

      break;

    case zombie:
      // Create a zombie wifi network using the chip id, can be used later to track individual zombies
      Serial << "Player is a zombie" << endl;

      char zombiessid[25];
      snprintf(zombiessid, 25, "Zombie-%06X", ESP.getChipId());
      WiFi.softAP(zombiessid, NULL, wifiChannel);
      break;
  }
}

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
int GameEngine::getTeamColour() {
  switch (team) {
    case human:
      // Serial << "Human Colour" << endl;
      return humanColour;
      break;

    case zombie:
      // Serial << "Zombie Colour" << endl;
      return zombieColour;
      break;
  }
  return 0;  // Needed to prevent compile errors
}
