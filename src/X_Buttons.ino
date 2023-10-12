void startButtons() {
  topButton.setClickTicks(350);    // 350
  topButton.setPressTicks(250);    // 250
  topButton.setDebounceTicks(50);  // 20

  topButton.attachClick(topButtonClicked);
  topButton.attachLongPressStart(topButtonHeld);
  // topButton.attachDoubleClick(doubleClick);

  bottomButton.setClickTicks(350);    // 350
  bottomButton.setPressTicks(250);    // 250
  bottomButton.setDebounceTicks(50);  // 20

  bottomButton.attachClick(bottomButtonClicked);
  bottomButton.attachLongPressStart(bottomButtonHeld);
  // bottomButton.attachDoubleClick(doubleClick);
}

void tickButtons() {
  topButton.tick();
  bottomButton.tick();
}

/*
 * Top Button
 */
void topButtonClicked() {
  Serial << "Top Button Clicked" << endl;

  if (inLobby) {
    team = !team;
  }

  if (gameRunning) {
    showTimeLeft();
  }

  if (gameFinished) {
    resetGame();
  }
}

void topButtonHeld() {
  Serial << "Top Button Held" << endl;
}

/*
 * Bottom Button
 */
void bottomButtonClicked() {
  Serial << "Bottom Button Clicked" << endl;

  if (inLobby) {
    inLobby = false;
    timeLobbyCountdownStarted = millis();
    startWiFi();
  }

  if (gameRunning) {
    showTimeLeft();
  }

  if (gameFinished) {
    resetGame();
  }
}

void bottomButtonHeld() {
  Serial << "Bottom Button Held" << endl;
}
