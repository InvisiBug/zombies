#include "game.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Arduino.h"
#include "Streaming.h"
using namespace std;

// need to inherit the led class and use it here as well as anywhere else using leds
Game::Game() {}

Game::Game(int totalLEDs, CRGB *currentLED) {
  this->totalLEDs = totalLEDs;
  this->currentLED = currentLED;
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
/*
  ! This game version doesnt use the whole "within a given distance, for a given time"
  ! It just checks if a zombie is within a given distance and then turns the human
*/
void Game::run(int &team) {
  int n = WiFi.scanNetworks(false, false, wifiChannel);
  if (log) Serial << n << endl;
  if (n < 1) {
    setAllLEDs(0x000000);

  } else {
    if (team == human) {
      currentDistance = maxDistance;

      for (int i = 0; i < n; i++) {
        if (WiFi.SSID(i).indexOf("Zombie") != -1) {
          int currentDistance = abs(WiFi.RSSI(i));

          if (log) Serial << "Current Distance" << currentDistance << "\tMax Distance:" << maxDistance << endl;  // Print out distance

          showDistance(currentDistance, humanColour);

          if (currentDistance < bitingDistance) {
            if (log) Serial << "Turned" << endl;
            WiFi.mode(WIFI_OFF);  // Turn off wifi before the animation (wifi takes a while to change state)

            for (int i = 0; i < 5; i++) {
              setAllLEDs(zombieColour);
              delay(500);
              setAllLEDs(0x000000);
              delay(500);
            }

            team = zombie;  // Team has been passed by reference from the game engine (super dodgy but it works for now)

            delay(100);
            startWiFi(team);
          }
        }
      }
    }

    else if (team == zombie)  // Zombie Team
    {
      currentDistance = maxDistance;

      for (int i = 0; i < n; i++) {
        if (WiFi.SSID(i).indexOf("Human") != -1) {
          // Human close
          currentDistance = abs(WiFi.RSSI(i));
          showDistance(currentDistance, zombieColour);
        }
      }
    }
  }
}

// TODO: Move to wifi class
void Game::startWiFi(int team) {
  // allOff();
  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  delay(100);  // Added to try and prevent crashing (Remove if not possible)

  /*
    Create the appropriate network for the team, adding the chip id on the end
    can be used for tracking players at a later date
  */
  switch (team) {
    case human:
      if (log) Serial << "Player is a human" << endl;

      char humanssid[25];
      snprintf(humanssid, 25, "Human-%06X", ESP.getChipId());
      WiFi.softAP(humanssid, NULL, wifiChannel);

      break;

    case zombie:
      // Create a zombie wifi network using the chip id, can be used later to track individual zombies
      if (log) Serial << "Player is a zombie" << endl;

      char zombiessid[25];
      snprintf(zombiessid, 25, "Zombie-%06X", ESP.getChipId());
      WiFi.softAP(zombiessid, NULL, wifiChannel);
      break;
  }
}

bool Game::gameTimeRemaining(int timeGameStarted, int totalGameTime) {
  int gameTimeLeft = totalGameTime - (millis() - timeGameStarted);

  // printGameTimeRemaining(gameTimeLeft);

  if (gameTimeLeft > 0) {
    return true;
  } else {
    return false;
  }
}

void Game::printGameTimeRemaining(int time) {
  int minsLeft = (time % (1000 * 60 * 60)) / (1000 * 60);
  int secondsLeft = ((time % (1000 * 60 * 60)) % (1000 * 60)) / 1000;
  Serial << "Remaining Time " << minsLeft << ":" << secondsLeft << endl;
}

void Game::showDistance(int currentDistance, int colour) {
  if (currentDistance >= maxDistance) {
    setAllLEDs(0x000000);

  } else {
    int indicatorLevel = map(currentDistance, maxDistance, bitingDistance, 0, totalLEDs);
    indicatorLevel = constrain(indicatorLevel, 0, totalLEDs);  // prevents crashing due to trying to show more than there actually are

    // Serial << "indicatorLevel" << indicatorLevel << endl;
    for (int i = 0; i < indicatorLevel; i++) {
      currentLED[i] = colour;
    }
    FastLED.show();

    for (int i = totalLEDs; i > indicatorLevel; i--) {
      currentLED[i] = 0x000000;
    }
    FastLED.show();
  }
}

int Game::getTeamColour(int team) {
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

void Game::setAllLEDs(int colour) {
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = colour;
  }
  FastLED.show();
}

// Buffer shit
// void runGameWithBuffer() {
//   int n = WiFi.scanNetworks(false, false, wifiChannel);

//   // Calculate the average of the buffer array
//   int bufferSum = 0;
//   for (int i = 0; i < 5; i++) {
//     bufferSum += buffer[i];
//   }
//   bufferAverage = bufferSum / 5;

//   if (team == human) {
//     currentDistance = maxDistance;

//     for (int i = 0; i < n; i++) {
//       if (WiFi.SSID(i) == "Zombie") {
//         // Zombie close

//         currentDistance = abs(WiFi.RSSI(i));

//         // and save the new distance if it is within the threshold of the average

//         showDistance(bufferAverage, humanColour);

//         // Save the value regardless if the buffer is empty (bitingDistance + 1)
//         // otherwise no values would get saved
//         if (buffer[bufferPos] == bitingDistance + 1) {
//           buffer[bufferPos] = currentDistance;
//           bufferPos++;
//         }

//         int threshold = 10;

//         // Save value if its within the threshold of the buffer average
//         if ((currentDistance > bufferAverage - threshold) && (currentDistance < bufferAverage + threshold)) {
//           Serial << "Buffer average: " << bufferAverage << " Save value : " << currentDistance << endl;

//           // Add each new distance to the buffer array
//           if (bufferPos < 5) {
//             buffer[bufferPos] = currentDistance;
//             bufferPos++;
//           } else {
//             bufferPos = 0;
//           }

//         } else {
//           Serial << "Buffer average: " << bufferAverage << " Discard value: " << currentDistance << endl;
//         }

//         // Serial << "Buffer average: " << bufferAverage << endl;

//         bufferSum = 0;

//         // Serial << "Buffer: [";
//         // for (int i = 0; i < 5; i++) {
//         //   Serial << "[" << buffer[i] << "]";
//         // }
//         // Serial << "]" << endl;
//       }
//     }

//     if (bufferAverage >= maxDistance) {
//       allOff();
//     }

//     checkIfBitten();

//     // Serial << currentDistance << endl;  // Print out distance
//   }

//   else if (team == zombie)  // Zombie Team
//   {
//     currentDistance = maxDistance;

//     for (int i = 0; i < n; i++) {
//       if (WiFi.SSID(i) == "Human") {
//         // Human close
//         currentDistance = abs(WiFi.RSSI(i));
//         showDistance(currentDistance, zombieColour);
//       }

//       if (currentDistance >= maxDistance) {
//         allOff();
//       }
//     }
//   }
// }

// void checkIfBitten() {
//   // Getting Bitten
//   if (bufferAverage < bitingDistance) {
//     // Been caught so start a timer
//     if (!beenCaught) {
//       timeCaught = millis();
//       beenCaught = true;
//     }

//     timeNow = millis() - timeCaught;
//     Serial << "Gonna get turned" << endl;

//     // Turned
//     if (timeNow >= timeTillTurned) {
//       Serial << "Turned" << endl;

//       for (int i = 0; i < 5; i++) {
//         setAllLEDs(zombieColour);
//         delay(500);
//         allOff();
//         delay(500);
//       }

//       team = zombie;
//       startWiFi();
//     }
//   }

//   // Not getting bitten
//   else {
//     timeCaught = 0;
//     beenCaught = false;
//   }
// }
