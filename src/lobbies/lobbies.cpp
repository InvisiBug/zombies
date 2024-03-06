#include "lobbies.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Arduino.h"
#include "Streaming.h"
using namespace std;

// need to inherit the led class and use it here as well as anywhere else using leds

Lobbies::Lobbies() {}

Lobbies::Lobbies(int totalLEDs, CRGB *currentLED) : LEDs(totalLEDs, currentLED) {
  this->totalLEDs = totalLEDs;
  this->currentLED = currentLED;
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

  if (currentMillis - lastMillis >= 250) {
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

/*
 * Flashes the team colour during the lobby countdown
 */
void Lobbies::countDownAnimation(int colour) {
  currentMillis = millis();

  if (currentMillis - lastMillis >= 250) {
    lastMillis = currentMillis;

    if (flipFlop) {
      setAllLEDs(colour);
    } else {
      setAllLEDs(0x000000);
    }
    flipFlop = !flipFlop;
  }
}

// TODO: Maybe include fire animation now that wifi is being handled else where
// ! Intentionally left here as will probs add the fire animation back in
void Lobbies::endGameLobby(int colour) {
  currentMillis = millis();

  if (currentMillis - lastMillis >= 250) {
    lastMillis = currentMillis;

    if (flipFlop) {
      setAllLEDs(0xff0000);
    } else {
      setAllLEDs(0x000000);
    }
    flipFlop = !flipFlop;
  }
}

/*
 * Timer for the lobby countdown
 * returns true if the countdown is finished
 * returns false if the countdown is still running
 * timeLobbyCountdownStarted is set by the button click
 */
bool Lobbies::lobbyCountdownFinished(int timeLobbyCountdownStarted, int lobbyCountdownTime) {
  int lobbyCountdownTimeRemaining = lobbyCountdownTime - (millis() - timeLobbyCountdownStarted);

  if (false) printTimeRemaining(lobbyCountdownTimeRemaining);

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
