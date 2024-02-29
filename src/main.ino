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

#define topButtonPin 12     // D6
#define bottomButtonPin 13  // D7
#define ledPin 14           // D5

#define time1Pin 5
#define time2Pin 4
#define time3Pin 3

#define totalLEDs 7
#define wifiChannel 10

// #define totalGameTime (1 * 60 * 1000)  // * Need to make adjustable
#define lobbyCountdownTime (60 * 1000)

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

enum Gamestate {
  lobby,
  countdown,
  game,
  postGame
};

// char* mode = "Game";

int team = 0;

// Options
int LEDBrightness = 20;   // As a percentage (saved as a dynamic variable to let us change later)
int bitingDistance = 50;  //? Set to 40 for actual game
int maxDistance = 100;
int timeTillTurned = 1.5 * 1000;

// Timer Variables
int timeNow = 0;
int timeCaught = 0;

int totalGameTime = 10;
int gameTimeLeft = 0;

int timeGameStarted = 0;
int lobbyCountdownTimeRemaining = lobbyCountdownTime;
int timeLobbyCountdownStarted = 0;

//! State machine variables (think very carefully before messing around with these, they can and will break everything)
bool beenCaught, lobbyCountdownRunning = false;

int currentDistance = 0;

int preGameLobbyCounter = totalLEDs - 1;  // leds are wired up counter clockwise
int postGameLobbyCounter = totalLEDs - 1;

int gameState = lobby;  //? Set to lobby for actual game

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

  //? Uncomment to set time based on header pins
  // if (!digitalRead(time1Pin)) {
  //   Serial << "Time is 10 min" << endl;
  //   totalGameTime = 10;
  //   setAllLEDs(0xff0000);
  // } else if (!digitalRead(time2Pin)) {
  //   Serial << "Time is 15 min" << endl;
  //   setAllLEDs(0x00ff00);
  //   totalGameTime = 15;
  // } else if (!digitalRead(time3Pin)) {
  //   Serial << "Time is 20 min" << endl;
  //   setAllLEDs(0x0000ff);
  //   totalGameTime = 20;
  // }

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
  FastLED.addLeds<WS2811, ledPin, GRB>(currentLED, totalLEDs);
  FastLED.setBrightness(LEDBrightness * 2.55);
  // FastLED.setCorrection(0xFFB0F0);
  // FastLED.setDither( 1 );

  startButtons();

  allOff();

  // allOff();
  // WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  // // // delay(100);  // Added to try and prevent crashing (Remove if not possible)

  // // // // WiFi.mode(WIFI_AP_STA);  // Wifi Modes (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA)

  // WiFi.softAP("Zombie", NULL, wifiChannel);

  // char APssid[25];

  // uint32_t chipid = ESP.getChipId();
  // snprintf(APssid, 25, "Zombie-%06X", chipid);
  // Serial.print("APssid:");
  // Serial.println(APssid);

  // WiFi.softAP(APssid, NULL, wifiChannel);
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

  switch (gameState) {
    case lobby:
      preGameLobby();
      break;

    case countdown:
      checkCountDownTime();
      break;

    case game:
      if (gameTimeRemaining()) {
        runGameWithoutBuffer();  // Still time remaining, play game
      } else {
        gameState = postGame;
      }
      break;

    case postGame:
      endGameLobby();
      break;
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
}