#include "gameEngine.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "./lobbies/lobbies.h"
#include "Arduino.h"
#include "Streaming.h"
using namespace std;

GameEngine::GameEngine(int totalLEDs, CRGB *currentLED) : lobbies(totalLEDs, currentLED) {
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
  lobbies.run();
  // switch (gameState) {
  //   case lobby:
  //     preGameLobby();
  //     break;

  //   case countdown:
  //     checkCountDownTime();
  //     break;

  //   case runGame:
  //     if (gameTimeRemaining()) {
  //       runGameWithoutBuffer();  // Still time remaining, play game
  //     } else {
  //       gameState = postGame;
  //     }
  //     break;

  //   case postGame:
  //     endGameLobby();
  //     break;
  // }
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
