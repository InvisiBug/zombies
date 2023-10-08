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
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <Streaming.h>
#include <WiFiClient.h>

// Effects
#include "ColourCycle.h"
#include "ColourFade.h"
#include "Crisscross.h"
#include "Fire.h"
#include "OneButton.h"
#include "Rainbow.h"

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
#define pushButton 13  // D7
#define ledPin 12      // D6

#define totalLEDs 8
#define wifiChannel 10

// #define totalGameTime (1 * 60 * 1000)
#define totalGameTime (5 * 1000)
#define totalPreStartTime (5 * 1000)

#define humanColour 0xba3939
#define zombieColour 0x094aef
#define randomColour 0x352465
#define timeColour 0x789812

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
OneButton button(pushButton, true);

// Effects
Fire fire(totalLEDs, currentLED);
Rainbow rainbow(totalLEDs, currentLED, 20);
ColourFade colourFade(totalLEDs, currentLED, 10);
Crisscross crissCross(totalLEDs, currentLED, 20);
ColourCycle colourCycle(totalLEDs, currentLED, 20);

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
char* team = "Human";
char* mode = "Game";

// Options
int LEDBrightness = 10;  // As a percentage (saved as a dynamic variable to let us change later)
int bitingDistance = 25;
int maxDistance = 50;
int timeTillTurned = 5 * 1000;

// Timer Variables
int timeNow = 0;
int timeCaught = 0;
int gameTimeLeft = totalGameTime;
int timeGameStarted = 0;
int preStartTimeLeft = totalPreStartTime;
int timeCountDownStarted = 0;

// State machine variables (think very carefully before messing around with these, they can and will break everything)
bool countDownFinished = false;
bool countDownRunning = false;
bool gameFinished = false;
bool gameRunning = false;
bool restartGame = false;
bool beenCaught = false;
bool gameReady = false;
bool started = false;

int currentDistance = 0;

int preGameLobbyCounter = totalLEDs - 1;  // leds are wired up counter clockwise
int postGameLobbyCounter = totalLEDs - 1;

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
  Serial.begin(115200);
  Serial << "Zombie Game" << endl;

  // Pin setups
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(zombiePin, INPUT_PULLUP);
  pinMode(humanPin, INPUT);
  pinMode(gamePin, INPUT);
  pinMode(ambientPin, INPUT);
  digitalWrite(LED_BUILTIN, off);

  // LEDs
  FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  FastLED.setBrightness(LEDBrightness * 2.55);
  FastLED.setCorrection(0xFFB0F0);
  // FastLED.setDither( 1 );

  // Effects
  colourCycle.begin();
  crissCross.begin();
  colourFade.begin();
  rainbow.begin();

  // Button options
  button.setDebounceTicks(20);  // 20
  button.setClickTicks(350);    // 350
  button.setPressTicks(250);    // 250

  // Setup Button Functions
  button.attachLongPressStart(longPress);
  button.attachDoubleClick(doubleClick);
  button.attachClick(click);

  allOff();

  // Setup the game
  // start(team);
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
  button.tick();
  // Serial << digitalRead(zombiePin) << endl;

  // Serial <<  "Zombie: "   << digitalRead(zombiePin);
  // Serial <<  " Human: "   << digitalRead(humanPin);
  // Serial <<  " Game: "    << digitalRead(gamePin);
  // Serial <<  " Ambient: " << digitalRead(ambientPin) << endl;

  // delay(100);

  if (digitalRead(gamePin))
    setupGame();
  else if (digitalRead(ambientPin))
    ambient();
  else
    allOff();
}