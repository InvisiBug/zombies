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

void GameEngine::reset() {
  setGameState(lobby);
  lobbies.reset();  // need to reset pre and post game lobby counters to (totalLEDs -1)
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
      lobbies.preGameLobby(getTeamColour());
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
        game.startWiFi(team);  // Only start wifi after the lobby has finished
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

// Buttons
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

void GameEngine::setGameState(int gameState) {
  this->gameState = gameState;
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
