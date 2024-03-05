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
// void lobbyCountdown() {
//   Serial << "Lobby Countdown" << endl;
//   if (lobbyCountdownRunning) {
//     checkCountDownTime();  // Start count down
//   } else {
//     //   // Countdown starts, take current time
//     lobbyCountdownRunning = true;
//     timeLobbyCountdownStarted = millis();
//   }
// }

void checkCountDownTime() {
  // Serial << "Check Count Down Time" << endl;
  if (lobbyCountdownFinished()) {
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
////////////////////////////////////////////////////////////////////////111
void preGameLobby() {
  // Serial << "Pre Game Lobby" << endl;

  // Set all LEDs to team colour
  setAllLEDs(getTeamColour());

  // Make one blank led move along the strip
  if (preGameLobbyCounter < totalLEDs - 1) {
    currentLED[preGameLobbyCounter] = 0x000000;
    preGameLobbyCounter++;

    FastLED.show();
    delay(100);
  } else {
    preGameLobbyCounter = 0;
  }
}

void endGameLobby() {
  Serial << "End Game Lobby" << endl;

  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = 0xff0000;
  }
  FastLED.show();
}
