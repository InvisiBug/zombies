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
#define topButtonPin 13  // D7
#define bottomButtonPin 14
#define ledPin 12  // D6

#define totalLEDs 8
#define wifiChannel 10

// #define totalGameTime (1 * 60 * 1000)
#define totalGameTime (5 * 1000)
#define totalPreStartTime (5 * 1000)
#define lobbyCountdownTime (5 * 1000)

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
OneButton topButton(topButtonPin, true);
OneButton bottomButton(bottomButtonPin, true);

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
int bitingDistance = 25;
int maxDistance = 50;
int timeTillTurned = 5 * 1000;

// Timer Variables
int timeNow = 0;
int timeCaught = 0;
int gameTimeLeft = totalGameTime;
int timeGameStarted = 0;
int preStartTimeLeft = totalPreStartTime;
int lobbyCountdownTimeRemaining = lobbyCountdownTime;

//! State machine variables (think very carefully before messing around with these, they can and will break everything)
// bool countDownFinished = false;
// bool countDownRunning = false;
// bool gameFinished = false;
// bool gameRunning = false;
// bool restartGame = false;
// bool beenCaught = false;
// bool gameReady = false;
// bool started = false;

bool countDownFinished,
    countDownRunning,
    gameFinished,
    gameRunning,
    restartGame,
    beenCaught,
    gameReady,
    started,
    inGame = false;

bool inLobby = true;
bool lobbyCountdownRunning = false;
int timeCountDownStarted = 0;
int timeLobbyCountdownStarted = 0;

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
  Serial.begin(9600);
  Serial << "Zombie Game" << endl;

  // Pin setups
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, off);

  // LEDs
  FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  FastLED.setBrightness(LEDBrightness * 2.55);
  FastLED.setCorrection(0xFFB0F0);
  // FastLED.setDither( 1 );

  startButtons();

  allOff();
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

  runTheGame();

  // for (int i = 0; i < totalLEDs; i++) {
  //   currentLED[i] = 0xff0000;
  // }
  // FastLED.show();
}