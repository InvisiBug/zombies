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
    gameState = game;
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

  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  // ! this introduces flicker, possible due to led timings
  // fire.run(15, 120, 20, true);

  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = 0xff0000;
  }
  FastLED.show();
}