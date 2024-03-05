#include "lobbies.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Arduino.h"
#include "Streaming.h"
using namespace std;

// need to inherit the led class and use it here as well as anywhere else using leds

Lobbies::Lobbies(int totalLEDs, CRGB *currentLED) {
  this->totalLEDs = totalLEDs;
  this->currentLED = currentLED;
}

void Lobbies::begin() {
}

void Lobbies::reset() {
  preGameLobbyPos = totalLEDs - 1;  // leds are wired up counter clockwise
  postGameLobbyPos = totalLEDs - 1;
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
void Lobbies::preGameLobby(int colour) {
  currentMillis = millis();

  if (currentMillis - lastMillis >= 100) {
    lastMillis = currentMillis;
    setAllLEDs(colour);


    // Make one blank led move along the strip
    if (preGameLobbyPos < totalLEDs) {
      currentLED[preGameLobbyPos] = 0x000000;

      FastLED.show();

      preGameLobbyPos++;
    } else {
      preGameLobbyPos = 0;
    }
  }
}

// TODO: Maybe include fire animation now that wifi is being handled else where
void Lobbies::endGameLobby(int colour) {
  Serial << "End Game Lobby" << endl;

  currentMillis = millis();

  if (currentMillis - lastMillis >= 250) {
    lastMillis = currentMillis;
    if (flipFlop) {
      for (int i = 0; i < totalLEDs; i++) {
        currentLED[i] = 0xff0000;
      }
      FastLED.show();
      flipFlop = !flipFlop;
    } else {
      for (int i = 0; i < totalLEDs; i++) {
        currentLED[i] = 0x000000;
      }
      FastLED.show();
      flipFlop = !flipFlop;
    }
  }}

  /*
    * Timer for the lobby countdown
    returns true if the countdown is finished
    returns false if the countdown is still running
    timeLobbyCountdownStarted is set by the button click
  */
  bool Lobbies::lobbyCountdownFinished(int timeLobbyCountdownStarted) {
    int lobbyCountdownTimeRemaining = countDownDuration - (millis() - timeLobbyCountdownStarted);

    // printTimeRemaining(lobbyCountdownTimeRemaining);

    if (lobbyCountdownTimeRemaining < 0) {
      return true;
    } else {
      return false;
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
  void Lobbies::setAllLEDs(int colour) {
    for (int i = 0; i < totalLEDs; i++) {
      currentLED[i] = colour;
    }
    FastLED.show();
  }

  void Lobbies::countDownAnimation(int colour) {
    // setAllLEDs(colour);

    for (int i = 0; i < 7; i++) {
      currentLED[i] = colour;
    }
    FastLED.show();

    delay(250);

    for (int i = 0; i < 7; i++) {
      currentLED[i] = 0x000000;
    }
    FastLED.show();

    delay(250);
  }
