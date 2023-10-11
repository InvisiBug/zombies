////////////////////////////////////////////////////////////////////////
//
//   #####                          #
//  #     #   ##   #    # ######    #        ####   ####  #  ####
//  #        #  #  ##  ## #         #       #    # #    # # #    #
//  #  #### #    # # ## # #####     #       #    # #      # #
//  #     # ###### #    # #         #       #    # #  ### # #
//  #     # #    # #    # #         #       #    # #    # # #    #
//   #####  #    # #    # ######    #######  ####   ####  #  ####
//
////////////////////////////////////////////////////////////////////////
void runTheGame() {
  // Serial << "Setup Game" << endl;
  if (inLobby) {
    preGameLobby();  // Players not ready, pre game lobby
  } else {
    lobbyCountdown();  // Players ready, begin countdown
  }
}

// Pre start countdown
void lobbyCountdown() {
  Serial << "Lobby Countdown" << endl;
  if (lobbyCountdownRunning) {
    checkCountDownTime();  // Start count down
  } else {
    //   // Countdown starts, take current time
    lobbyCountdownRunning = true;
    timeLobbyCountdownStarted = millis();
  }
}

void checkCountDownTime() {
  // Serial << "Check Count Down Time" << endl;
  if (lobbyCountdownFinished())
    runGame();
  else
    countDownAnimation();  // count down animation;
}

void runGame() {
  Serial << "Game running" << endl;
  if (gameRunning)
    checkGameTime();  // Game running
  else                // Game starts, take current time
  {
    gameRunning = true;
    timeGameStarted = millis();
  }
}

void checkGameTime() {
  if (gameTimeRemaining())
    game();  // Still time remaining, play game
  else
    endGame();  // Game finished
}

void endGame() {
  if (restartGame)
    resetGame();  // Restarting game
  else
    endGameLobby();  // Game finished, final lobby
}

////////////////////////////////////////////////////////////////////////
//
// #
// #        ####  #####  #####  # ######  ####
// #       #    # #    # #    # # #      #
// #       #    # #####  #####  # #####   ####
// #       #    # #    # #    # # #           #
// #       #    # #    # #    # # #      #    #
// #######  ####  #####  #####  # ######  ####
//
////////////////////////////////////////////////////////////////////////
void preGameLobby() {
  // Serial << "Pre Game Lobby" << endl;
  // Set team colour

  // Set all LEDs to team colour
  setAllLEDs(getTeamColour());

  // Make one blank led move along the strip
  if (preGameLobbyCounter >= 0) {
    currentLED[preGameLobbyCounter] = 0x000000;
    preGameLobbyCounter--;

    FastLED.show();
    delay(100);
  } else {
    preGameLobbyCounter = totalLEDs - 1;
  }
}

void endGameLobby() {
  Serial << "End Game Lobby" << endl;

  gameFinished = true;

  setAllLEDs(0x000000);
  delay(1000);
  setAllLEDs(getTeamColour());
  delay(1500);
}