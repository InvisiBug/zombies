#include "lobbies.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Arduino.h"
#include "Streaming.h"
using namespace std;

Lobbies::Lobbies(int totalLEDs, CRGB *currentLED) {
  this->totalLEDs = totalLEDs;
  this->currentLED = currentLED;
}

void Lobbies::begin() {
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
void Lobbies::run() {
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
