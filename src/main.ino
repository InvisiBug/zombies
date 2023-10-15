////////////////////////////////////////////////////////////////////////
//  Matthew Kavanagh
//
//  Zombies
//  27/02/2019
//  *********
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

#include "effects/Fire.h"

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
#define on LOW
#define off HIGH

#define topButtonPin 13     // D7
#define bottomButtonPin 14  // D5
#define ledPin 12           // D6
#define time1Pin 2          // D4
#define time2Pin 4          // D2
#define time3Pin 5          // D1

#define totalLEDs 8
#define wifiChannel 10

#define totalGameTime (1 * 60 * 1000)
#define lobbyCountdownTime (5 * 1000)

#define humanColour 0x0000ff
#define zombieColour 0x00ff00
#define randomColour 0x352465
#define timeColour 0xacacac

#define zombiePin 16  // D0
#define humanPin 14   // D5
#define gamePin 5     // D1
#define ambientPin 4  // D2

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

// Effects
// Crisscross crissCross(totalLEDs, currentLED, 20);
Fire fire(totalLEDs, currentLED);

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

char* mode = "Game";

int team = 0;

// Options
int LEDBrightness = 10;  // As a percentage (saved as a dynamic variable to let us change later)
int bitingDistance = 30;
int maxDistance = 50;
int timeTillTurned = 2 * 1000;

// Timer Variables
int timeNow = 0;
int timeCaught = 0;
int gameTimeLeft = totalGameTime;
int timeGameStarted = 0;
int lobbyCountdownTimeRemaining = lobbyCountdownTime;

//! State machine variables (think very carefully before messing around with these, they can and will break everything)
bool gameFinished,
    gameRunning,
    beenCaught,
    started,
    inGame = false;

bool inLobby = true;
bool lobbyCountdownRunning = false;
int timeCountDownStarted = 0;
int timeLobbyCountdownStarted = 0;

int currentDistance = 0;

int preGameLobbyCounter = totalLEDs - 1;  // leds are wired up counter clockwise
int postGameLobbyCounter = totalLEDs - 1;

bool endGameLobbyDirection = 0;

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

  // Pin setups
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(time1Pin, INPUT_PULLUP);
  pinMode(time2Pin, INPUT_PULLUP);
  pinMode(time3Pin, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, off);

  // LEDs
  FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  FastLED.setBrightness(LEDBrightness * 2.55);
  // FastLED.setCorrection(0xFFB0F0);
  // FastLED.setDither( 1 );

  startButtons();

  allOff();

  // allOff();
  // WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  // delay(100);  // Added to try and prevent crashing (Remove if not possible)

  // // WiFi.mode(WIFI_AP_STA);  // Wifi Modes (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA)

  // WiFi.softAP("Zombie", NULL, wifiChannel);
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
  tickButtons();

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

  if (!digitalRead(time1Pin)) {
    Serial << "Time is 10 min" << endl;
  } else if (!digitalRead(time2Pin)) {
    Serial << "Time is 15 min" << endl;
  } else if (!digitalRead(time3Pin)) {
    Serial << "Time is 20 min" << endl;
  }

  runTheGame();

  // for (int i = 0; i < totalLEDs; i++) {
  //   currentLED[i] = 0xff0000;
  // }
  // FastLED.show();
}