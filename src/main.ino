////////////////////////////////////////////////////////////////////////
//
//  ###
//   #  #    #  ####  #      #    # #####  ######  ####
//   #  ##   # #    # #      #    # #    # #      #
//   #  # #  # #      #      #    # #    # #####   ####
//   #  #  # # #      #      #    # #    # #           #
//   #  #   ## #    # #      #    # #    # #      #    #
//  ### #    #  ####  ######  ####  #####  ######  ####
//
////////////////////////////////////////////////////////////////////////
// #include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <OneButton.h>
#include <Streaming.h>
#include <WiFiClient.h>

#include "gameEngine/gameEngine.h"

////////////////////////////////////////////////////////////////////////
//
//  ######
//  #     # ###### ###### # #    # # ##### #  ####  #    #  ####
//  #     # #      #      # ##   # #   #   # #    # ##   # #
//  #     # #####  #####  # # #  # #   #   # #    # # #  #  ####
//  #     # #      #      # #  # # #   #   # #    # #  # #      #
//  #     # #      #      # #   ## #   #   # #    # #   ## #    #
//  ######  ###### #      # #    # #   #   #  ####  #    #  ####
//
////////////////////////////////////////////////////////////////////////

#define topButtonPin 12     // D6
#define bottomButtonPin 13  // D7
#define ledPin 14           // D5

#define data 4   // D2
#define clock 5  // D1

#define time1Pin 5
#define time2Pin 4
#define time3Pin 3

#define totalLEDs 7
#define wifiChannel 10

// #define totalGameTime (1 * 60 * 1000)  // * Need to make adjustable
// #define lobbyCountdownTime (60 * 1000)

#define humanColour 0x0000ff
#define zombieColour 0x008800
#define randomColour 0x352465
#define timeColour 0xacacac

////////////////////////////////////////////////////////////////////////
//
//  #     #
//  #     #   ##   #####  #####  #    #   ##   #####  ######
//  #     #  #  #  #    # #    # #    #  #  #  #    # #
//  ####### #    # #    # #    # #    # #    # #    # #####
//  #     # ###### #####  #    # # ## # ###### #####  #
//  #     # #    # #   #  #    # ##  ## #    # #   #  #
//  #     # #    # #    # #####  #    # #    # #    # ######
//
////////////////////////////////////////////////////////////////////////
// LEDs
CRGB currentLED[totalLEDs];

// Button
OneButton topButton(topButtonPin, true);
OneButton bottomButton(bottomButtonPin, true);

GameEngine gameEngine(totalLEDs, currentLED);

// Effects

////////////////////////////////////////////////////////////////////////
//
//  #     #
//  #     #   ##   #####  #   ##   #####  #      ######  ####
//  #     #  #  #  #    # #  #  #  #    # #      #      #
//  #     # #    # #    # # #    # #####  #      #####   ####
//   #   #  ###### #####  # ###### #    # #      #           #
//    # #   #    # #   #  # #    # #    # #      #      #    #
//     #    #    # #    # # #    # #####  ###### ######  ####
//
////////////////////////////////////////////////////////////////////////
enum Team {
  human,
  zombie
};

enum Mode {
  game,
  beacon,
  dev
};

enum Gamestate {
  lobby,
  countdown,
  runGame,
  postGame
};

// char* mode = "Game";

int gameState = lobby;  //? Set to lobby for actual game
int team = zombie;
int mode = dev;

// Options
int LEDBrightness = 10;   // As a percentage (saved as a dynamic variable to let us change later)
int bitingDistance = 50;  //? Set to 40 for actual game
int maxDistance = 100;
int timeTillTurned = 1.5 * 1000;

// Timer Variables
int timeNow = 0;
int timeCaught = 0;

int totalGameTime = 10;  // in mins
int gameTimeLeft = 0;

int lobbyCountdownTime = (30 * 1000);
int timeGameStarted = 0;
int lobbyCountdownTimeRemaining = lobbyCountdownTime;
int timeLobbyCountdownStarted = 0;

//! State machine variables (think very carefully before messing around with these, they can and will break everything)
bool beenCaught, lobbyCountdownRunning = false;

int currentDistance = 0;

int preGameLobbyCounter = totalLEDs - 1;  // leds are wired up counter clockwise
int postGameLobbyCounter = totalLEDs - 1;

// Set the buffer to biting distance + 1 to handle not finding the zombie network
int buffer[5] = {bitingDistance + 1, bitingDistance + 1, bitingDistance + 1, bitingDistance + 1, bitingDistance + 1};
int bufferPos = 0;
int bufferAverage = 0;

////////////////////////////////////////////////////////////////////////
//
//  ######                                                #####
//  #     # #####   ####   ####  #####    ##   #    #    #     # #####   ##   #####  ##### #    # #####
//  #     # #    # #    # #    # #    #  #  #  ##  ##    #         #    #  #  #    #   #   #    # #    #
//  ######  #    # #    # #      #    # #    # # ## #     #####    #   #    # #    #   #   #    # #    #
//  #       #####  #    # #  ### #####  ###### #    #          #   #   ###### #####    #   #    # #####
//  #       #   #  #    # #    # #   #  #    # #    #    #     #   #   #    # #   #    #   #    # #
//  #       #    #  ####   ####  #    # #    # #    #     #####    #   #    # #    #   #    ####  #
//
////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Serial << "Zombie Game" << endl;

  gameTimeLeft = totalGameTime;

  /////////////
  // Pin setups
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(time1Pin, INPUT_PULLUP);
  pinMode(time2Pin, INPUT_PULLUP);
  pinMode(time3Pin, INPUT_PULLUP);

  ///////
  // LEDs
  // FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  FastLED.addLeds<APA102, data, clock, BGR, DATA_RATE_MHZ(24)>(currentLED, totalLEDs);  // * May not need to specify the data rate
  FastLED.setBrightness(LEDBrightness * 2.55);
  // FastLED.setCorrection(0xFFB0F0);
  // FastLED.setDither( 1 );

  startButtons();

  allOff();

  WiFi.mode(WIFI_OFF);

}

///////////////////////////////////////////////////////////////////////
//
//  #     #                    ######
//  ##   ##   ##   # #    #    #     # #####   ####   ####  #####    ##   #    #
//  # # # #  #  #  # ##   #    #     # #    # #    # #    # #    #  #  #  ##  ##
//  #  #  # #    # # # #  #    ######  #    # #    # #      #    # #    # # ## #
//  #     # ###### # #  # #    #       #####  #    # #  ### #####  ###### #    #
//  #     # #    # # #   ##    #       #   #  #    # #    # #   #  #    # #    #
//  #     # #    # # #    #    #       #    #  ####   ####  #    # #    # #    #
//
///////////////////////////////////////////////////////////////////////
void loop(void) {
  // runGameWithoutBuffer();
  tickButtons();

  // LEDTest();

  switch (mode) {
    case dev:
      // Serial << "Dev" << endl;

      gameEngine.run();

  //      Serial << "On" << endl;
  // for (int i = 0; i < totalLEDs; i++) {
  //   currentLED[i] = 0xff0000;
  // }
  // FastLED.show();
  // delay(500);

  // Serial << "Off" << endl;
  // for (int i = 0; i < totalLEDs; i++) {
  //   currentLED[i] = 0x000000;
  // }
  // FastLED.show();
  // delay(500);
      // for (int i = 0; i < 7; i++) {
      //   currentLED[i] = 0x0000ff;
      // }
      // FastLED.show();

      // delay(250);

      // for (int i = 0; i < 7; i++) {
      //   currentLED[i] = 0x000000;
      // }
      // FastLED.show();

      // delay(250);
      break;

    case game:
      Serial << gameEngine.gameState << endl;
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
      break;

    case beacon:

      int n = WiFi.scanNetworks(false, false, wifiChannel);

      if (team == human) {
        //* Maybe make it only display the closest zombie
        currentDistance = maxDistance;
        // Serial << "human" << endl;

        for (int i = 0; i < n; i++) {
          if (WiFi.SSID(i).indexOf("Zombie") != -1) {
            int currentDistance = abs(WiFi.RSSI(i));

            // Serial << currentDistance << endl;  // Print out distance

            showDistance(currentDistance, humanColour);
          }
        }
      } else if (team == zombie) {
        //* Maybe make it only display the closest zombie
        currentDistance = maxDistance;
        // Serial << "zombie" << endl;

        for (int i = 0; i < n; i++) {
          if (WiFi.SSID(i).indexOf("Human") != -1) {
            int currentDistance = abs(WiFi.RSSI(i));

            // Serial << currentDistance << endl;  // Print out distance

            showDistance(currentDistance, zombieColour);
          }
        }
      }
      break;
  }
}

// Serial.print("Scan start ... ");
// int n = WiFi.scanNetworks();
// Serial.print(n);
// Serial.println(" network(s) found");
// for (int i = 0; i < n; i++) {
//   String ssid = WiFi.SSID(i);
//   String id = ssid.substring(7, 13);

//   if (ssid.indexOf("Zombie") != -1) {
//     Serial << "Zombie found: " << id << endl;
//   }
//   Serial << ssid << " " << abs(WiFi.RSSI(i)) << endl;
// }
// Serial.println();

// delay(5000);

// Serial << digitalRead(zombiePin) << endl;

// Serial <<  "Zombie: "   << digitalRead(zombiePin);
// Serial <<  " Human: "   << digitalRead(humanPin);
// Serial <<  " Game: "    << digitalRead(gamePin);
// Serial <<  " Ambient: " << digitalRead(ambientPin) << endl;

// delay(100);

// Serial.print("Scan start ... ");
// int n = WiFi.scanNetworks();
// Serial.print(n);
// Serial.println(" network(s) found");
// for (int i = 0; i < n; i++) {
//   Serial.println(WiFi.SSID(i));
// }
// Serial.println();

// delay(5000);

// runTheGame();

// for (int i = 0; i < totalLEDs; i++) {
//   currentLED[i] = 0xff0000;
// }
// FastLED.show();
