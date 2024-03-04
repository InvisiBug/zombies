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

  switch (mode) {
    case game:
      switch (gameState) {
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
  Serial << "Top Button Held" << endl;
}

/*
 * Bottom Button
 */
void bottomButtonClicked() {
  Serial << "Bottom Button Clicked" << endl;

  switch (mode) {
    case game:
      switch (gameState) {
        case lobby:
          timeLobbyCountdownStarted = millis();
          startWiFi();
          gameState = countdown;
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
  Serial << "Bottom Button Held" << endl;
}
