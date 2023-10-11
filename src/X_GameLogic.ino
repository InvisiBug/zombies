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
  if (!gameRunning) {
    if (inLobby) {
      preGameLobby();  // Players not ready, pre game lobby
    } else {
      lobbyCountdown();  // Players ready, begin countdown
    }
  } else {
    if (gameTimeRemaining()) {
      game();  // Still time remaining, play game
    } else {
      endGameLobby();  // Game finished, final lobby
    }
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
  if (lobbyCountdownFinished()) {
    // runGame();
    gameRunning = true;
    timeGameStarted = millis();
  } else {
    countDownAnimation();  // count down animation;
  }
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
  // delay(1000);
  setAllLEDs(getTeamColour());
  // delay(1500);
}