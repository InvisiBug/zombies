////////////////////////////////////////////////////////////////////////
//  Matthew Kavanagh
//
//  Zombies
//  27/02/2019
//  *********
////////////////////////////////////////////////////////////////////////
//
//  #####
// #     # #####   ##   #####  ##### #    # #####
// #         #    #  #  #    #   #   #    # #    #
//  #####    #   #    # #    #   #   #    # #    #
//       #   #   ###### #####    #   #    # #####
// #     #   #   #    # #   #    #   #    # #
//  #####    #   #    # #    #   #    ####  #
//
////////////////////////////////////////////////////////////////////////
/*
 * Started by the bottom button click when in the lobby
 */
void startWiFi() {
  // allOff();
  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  delay(100);  // Added to try and prevent crashing (Remove if not possible)

  // WiFi.mode(WIFI_AP_STA); // Wifi Modes (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA)
  WiFi.softAP("Zombies", NULL, wifiChannel);  //* Using a specific wifi channel, makes scanning later much faster

  switch (team) {
    case human:
      Serial << "Player is a human" << endl;

      char humanssid[25];
      snprintf(humanssid, 25, "Human-%06X", ESP.getChipId());
      WiFi.softAP(humanssid, NULL, wifiChannel);

      break;

    case zombie:
      // Create a zombie wifi network using the chip id, can be used later to track individual zombies
      Serial << "Player is a zombie" << endl;

      char zombiessid[25];
      snprintf(zombiessid, 25, "Zombie-%06X", ESP.getChipId());
      WiFi.softAP(zombiessid, NULL, wifiChannel);
      break;
  }
}

////////////////////////////////////////////////////////////////////////
//
// #       ####### ######
// #       #       #     #  ####
// #       #       #     # #
// #       #####   #     #  ####
// #       #       #     #      #
// #       #       #     # #    #
// ####### ####### ######   ####
//
////////////////////////////////////////////////////////////////////////
void setAllLEDs(int colour) {
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = colour;
  }
  FastLED.show();
}

void countDownAnimation() {
  setAllLEDs(getTeamColour());

  delay(150);

  allOff();

  delay(150);
}

int getTeamColour() {
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

void showTimeLeft() {
  // int indicatorLevel = map(gameTimeLeft, 0, totalGameTime - 100, 0, totalLEDs);
  int indicatorLevel = map(gameTimeLeft, 0, (totalGameTime * 60 * 1000) - 100, 0, totalLEDs);

  Serial << "Indicator Level: " << indicatorLevel << endl;

  // showDistance(indicatorLevel, timeColour);
  for (int i = 0; i < indicatorLevel; i++) {
    currentLED[i] = timeColour;
  }
  FastLED.show();

  for (int i = totalLEDs; i > indicatorLevel; i--) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
  delay(500);

  allOff();
}

void showDistance(int currentDistance, int colour) {
  int indicatorLevel = map(currentDistance, maxDistance, bitingDistance, 0, totalLEDs);
  indicatorLevel = constrain(indicatorLevel, 0, totalLEDs);  // prevents crashing due to trying to show more than there actually are

  for (int i = 0; i < indicatorLevel; i++) {
    currentLED[i] = colour;
  }
  FastLED.show();

  for (int i = totalLEDs; i > indicatorLevel; i--) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}

void allOff() {
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}

////////////////////////////////////////////////////////////////////////
//
// #######
//    #    # #    # ###### #####   ####
//    #    # ##  ## #      #    # #
//    #    # # ## # #####  #    #  ####
//    #    # #    # #      #####       #
//    #    # #    # #      #   #  #    #
//    #    # #    # ###### #    #  ####
//
////////////////////////////////////////////////////////////////////////
bool gameTimeRemaining() {
  gameTimeLeft = (totalGameTime * 60 * 1000) - (millis() - timeGameStarted);

  // printTimeRemaining(gameTimeLeft);

  if (gameTimeLeft > 0) {
    return true;
  } else {
    return false;
  }
}

/*
  * Timer for the lobby countdown
  returns true if the countdown is finished
  returns false if the countdown is still running
  timeLobbyCountdownStarted is set by the button click
*/
bool lobbyCountdownFinished() {
  lobbyCountdownTimeRemaining = lobbyCountdownTime - (millis() - timeLobbyCountdownStarted);

  // printTimeRemaining(lobbyCountdownTimeRemaining);

  if (lobbyCountdownTimeRemaining < 0) {
    return true;
  } else {
    return false;
  }
}

////////////////////////////////////////////////////////////////////////
//
//  #     #
//  ##   ## #  ####   ####
//  # # # # # #      #    #
//  #  #  # #  ####  #
//  #     # #      # #
//  #     # # #    # #    #
//  #     # #  ####   ####
//
////////////////////////////////////////////////////////////////////////
void printTimeRemaining(int time) {
  int minsLeft = (time % (1000 * 60 * 60)) / (1000 * 60);
  int secondsLeft = ((time % (1000 * 60 * 60)) % (1000 * 60)) / 1000;
  Serial << "Remaining Time " << minsLeft << ":" << secondsLeft << endl;
}

void resetGame() {
  allOff();

  gameState = lobby;

  gameTimeLeft = totalGameTime;

  timeGameStarted = 0;
  lobbyCountdownTimeRemaining = lobbyCountdownTime;

  preGameLobbyCounter = totalLEDs - 1;
  postGameLobbyCounter = totalLEDs - 1;
}