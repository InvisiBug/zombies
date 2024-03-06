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
  // currentMillis = millis();

  // if (currentMillis - lastMillis >= 75) {
  //   lastMillis = currentMillis;
  //   setAllLEDs(colour);

  //   // Make one blank led move along the strip
  //   if (preGameLobbyPos < totalLEDs) {
  //     currentLED[preGameLobbyPos] = 0x000000;

  //     FastLED.show();

  //     preGameLobbyPos++;
  //   } else {
  //     preGameLobbyPos = 0;
  //   }
  // }

  currentMillis = millis();

  if (currentMillis - lastMillis >= 75) {
    lastMillis = currentMillis;
    setAllLEDs(colour);

    // Make one blank led move along the strip
    if (preGameLobbyPos < totalLEDs && !flipFlop) {
      currentLED[preGameLobbyPos] = 0x000000;

      FastLED.show();

      preGameLobbyPos++;
    } else if (preGameLobbyPos >= 0 && flipFlop) {
      currentLED[preGameLobbyPos] = 0x000000;

      FastLED.show();

      preGameLobbyPos--;
    }

    // If the LED has reached the end of the strip, flip the direction
    if (preGameLobbyPos == totalLEDs - 1) {  // "Blank" LED fell off the end of the strip (-1) corrects that
      flipFlop = true;
    }
    // If the LED has reached the start of the strip, flip the direction
    else if (preGameLobbyPos == 0) {
      flipFlop = false;
    }
  }
}

/*
 * Flashes the team colour during the lobby countdown
 */
void Lobbies::countDownAnimation(int colour) {
  currentMillis = millis();

  if (currentMillis - lastMillis >= 10) {  // Adjust this value to change the speed of the fade
    lastMillis = currentMillis;

    if (flipFlop) {
      brightness += 17;  // Adjust this value to change the step size of the fade
      if (brightness >= 255) {
        flipFlop = !flipFlop;
      }
    } else {
      brightness -= 17;  // Adjust this value to change the step size of the fade
      if (brightness <= 0) {
        flipFlop = !flipFlop;
      }
    }

    // Apply the brightness to the colour
    int r = ((colour >> 16) & 0xFF) * brightness / 255;
    int g = ((colour >> 8) & 0xFF) * brightness / 255;
    int b = (colour & 0xFF) * brightness / 255;
    int fadedColour = (r << 16) | (g << 8) | b;

    setAllLEDs(fadedColour);
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

void Lobbies::printTimeRemaining(int time) {
  int minsLeft = (time % (1000 * 60 * 60)) / (1000 * 60);
  int secondsLeft = ((time % (1000 * 60 * 60)) % (1000 * 60)) / 1000;
  Serial << "Remaining Time " << minsLeft << ":" << secondsLeft << endl;
}
