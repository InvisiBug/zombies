void startButtons() {
  topButton.setClickMs(350);    // 350
  topButton.setPressMs(250);    // 250
  topButton.setDebounceMs(50);  // 20

  topButton.attachClick(topButtonClicked);
  topButton.attachLongPressStart(topButtonHeld);
  // topButton.attachDoubleClick(doubleClick);

  bottomButton.setClickMs(350);    // 350
  bottomButton.setPressMs(250);    // 250
  bottomButton.setDebounceMs(50);  // 20

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
  Serial << "Hardware, Top Button Clicked" << endl;

  switch (mode) {
    case dev:
      gameEngine.topButtonClicked();
      break;

    case game:
      switch (gameEngine.gameState) {
        case lobby:
          team = !team;
          break;
        case runGame:
          showTimeLeft();
          break;
        case postGame:
          resetGame();
          break;
      }
      break;

    case beacon:
      team = !team;
      startWiFi();
      break;
  }
}

void topButtonHeld() {
  Serial << "Hardware, Top Button Held" << endl;
}

/*
 * Bottom Button
 */
void bottomButtonClicked() {
  Serial << "Hardware, Bottom Button Clicked" << endl;

  switch (mode) {
    case dev:
      gameEngine.bottomButtonClicked();
      break;

    case game:
      switch (gameState) {
        case lobby:
          // gameEngine.startLobbyCountdown();
          // timeLobbyCountdownStarted = millis();
          // startWiFi();
          // gameState = countdown;
          break;
        case runGame:
          showTimeLeft();
          break;
        case postGame:
          resetGame();
          break;
      }
      break;

    case beacon:
      // cant do led stuff here, probs to do with the button interupts

      team = !team;
      startWiFi();
      break;
  }
}

void bottomButtonHeld() {
  Serial << "Hardware, Bottom Button Held" << endl;
}
