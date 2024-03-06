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
      lobbies.preGameLobby(game.getTeamColour(team));
      break;

    case countdown:
      /*
       * if the timers not finished, run the animation
       * otherwise change the game state to "runGame" and note the time started
       */
      if (!lobbies.lobbyCountdownFinished(timeLobbyCountdownStarted, lobbyCountdownTime)) {
        lobbies.countDownAnimation(game.getTeamColour(team));  // count down animation;

      } else {
        setGameState(runGame);
        timeGameStarted = millis();
        game.startWiFi(team);  // Only start wifi after the lobby has finished
        game.setAllLEDs(0x000000);
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
      lobbies.endGameLobby(game.getTeamColour(team));
      break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
// ######
// #     # #    # ##### #####  ####  #    #  ####
// #     # #    #   #     #   #    # ##   # #
// ######  #    #   #     #   #    # # #  #  ####
// #     # #    #   #     #   #    # #  # #      #
// #     # #    #   #     #   #    # #   ## #    #
// ######   ####    #     #    ####  #    #  ####
//
//////////////////////////////////////////////////////////////////////////////
void GameEngine::topButtonClicked() {
  Serial << "Game Engine, Top Button Clicked" << endl;

  switch (gameState) {
    case lobby:
      team = !team;
      break;

    case countdown:
      Serial << "Show countdown time remaining" << endl;
      showTimeLeft(timeLobbyCountdownStarted, lobbyCountdownTime);
      break;

    case runGame:
      Serial << "Show game time remaining" << endl;
      showTimeLeft(timeGameStarted, totalGameTime);
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

    case countdown:
      Serial << "Show countdown time remaining" << endl;
      showTimeLeft(timeLobbyCountdownStarted, lobbyCountdownTime);
      break;

    case runGame:
      Serial << "Show game time remaining" << endl;
      showTimeLeft(timeGameStarted, totalGameTime);
      // * LED class show time left, will need to be passed the time remaining
      break;

    case postGame:
      Serial << "Game Engine, Bottom Button Clicked, Post Game" << endl;
      reset();
      break;
  }
}

void GameEngine::showTimeLeft(int startTime, int totalTime) {
  game.setAllLEDs(0x000000);

  int timeLeft = totalTime - (millis() - startTime);

  Serial << "Game time left: " << timeLeft << endl;

  int indicatorLevel = map(timeLeft, 0, totalTime - 100, 0, totalLEDs);  // ! Not sure what the -100 is here

  Serial << "Indicator Level: " << indicatorLevel << endl;

  game.setAllLEDs(timeColour);

  for (int i = totalLEDs; i > indicatorLevel; i--) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
  delay(500);

  game.setAllLEDs(0x000000);
}

void GameEngine::setGameState(int gameState) {
  this->gameState = gameState;
}
