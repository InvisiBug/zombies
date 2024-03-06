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
enum Team {  // Needed
  human,
  zombie
};

enum Mode {
  game,
  beacon,
  dev
};

int team = human;

int mode = game;

// * All stuff needed for the beacon mode
int LEDBrightness = 100;   // As a percentage (saved as a dynamic variable to let us change later)
int bitingDistance = 50;  //? Set to 40 for actual game

int maxDistance = 100;
int currentDistance = 0;

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

  /////////////
  // Pin setups
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(time1Pin, INPUT_PULLUP);
  pinMode(time2Pin, INPUT_PULLUP);
  pinMode(time3Pin, INPUT_PULLUP);

  ///////
  // LEDs
  FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  // FastLED.addLeds<APA102, data, clock, BGR, DATA_RATE_MHZ(24)>(currentLED, totalLEDs);  // * May not need to specify the data rate
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
  // tickButtons();

      Serial << "Hello" << endl;
  // switch (mode) {
  //   case dev:
  //     gameEngine.run();
  //     break;

  //   case game:
  //     gameEngine.run();
  //     break;

  //   case beacon:

  //     int n = WiFi.scanNetworks(false, false, wifiChannel);

  //     if (team == human) {
  //       //* Maybe make it only display the closest zombie
  //       currentDistance = maxDistance;
  //       // Serial << "human" << endl;

  //       for (int i = 0; i < n; i++) {
  //         if (WiFi.SSID(i).indexOf("Zombie") != -1) {
  //           int currentDistance = abs(WiFi.RSSI(i));

  //           // Serial << currentDistance << endl;  // Print out distance

  //           showDistance(currentDistance, humanColour);
  //         }
  //       }
  //     } else if (team == zombie) {
  //       //* Maybe make it only display the closest zombie
  //       currentDistance = maxDistance;
  //       // Serial << "zombie" << endl;

  //       for (int i = 0; i < n; i++) {
  //         if (WiFi.SSID(i).indexOf("Human") != -1) {
  //           int currentDistance = abs(WiFi.RSSI(i));

  //           // Serial << currentDistance << endl;  // Print out distance

  //           showDistance(currentDistance, zombieColour);
  //         }
  //       }
  //     }
  //     break;
  // }
}
