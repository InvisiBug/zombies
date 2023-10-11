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

  WiFi.mode(WIFI_AP_STA);  // Wifi Modes (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA)

  switch (team) {
    case human:
      Serial << "Player is a human" << endl;
      WiFi.softAP("Human", NULL, wifiChannel);
      break;
    case zombie:
      Serial << "Player is a zombie" << endl;
      WiFi.softAP("Zombie", NULL, wifiChannel);
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

void countDownAnimation()  // Add random selection
{
  // Set all LEDs to team colour
  setAllLEDs(getTeamColour());

  delay(150);

  allOff();

  delay(150);
}

int getTeamColour() {
  switch (team) {
    case human:
      return humanColour;
      break;
    case zombie:
      return zombieColour;
      break;
  }
  return 0;  // Needed to prevent compile errors
}

void showTimeLeft() {
  int indicatorLevel = map(gameTimeLeft, 0, totalGameTime - 100, 0, totalLEDs);

  showDistance(indicatorLevel, timeColour);
  delay(500);

  allOff();
}

void showDistance(int indicatorLevel, int colour) {
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
  gameTimeLeft = totalGameTime - (millis() - timeGameStarted);

  printTimeRemaining(gameTimeLeft);

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

  gameTimeLeft = totalGameTime;  // 45 mins

  timeGameStarted = 0;
  timeCountDownStarted = 0;
  lobbyCountdownTimeRemaining = lobbyCountdownTime;

  gameRunning = false;

  gameFinished = false;
  inLobby = true;
  preGameLobbyCounter = totalLEDs - 1;
  postGameLobbyCounter = totalLEDs - 1;
}