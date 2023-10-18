////////////////////////////////////////////////////////////////////////
//
//   #####
//  #     #   ##   #    # ######
//  #        #  #  ##  ## #
//  #  #### #    # # ## # #####
//  #     # ###### #    # #
//  #     # #    # #    # #
//   #####  #    # #    # ######
//
////////////////////////////////////////////////////////////////////////
void runGame() {
  int n = WiFi.scanNetworks(false, false, wifiChannel);

  if (team == human) {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == "Zombie") {
        // Zombie close
        currentDistance = abs(WiFi.RSSI(i));
        showDistance(currentDistance, humanColour);
      }
    }

    if (currentDistance >= maxDistance) {
      allOff();
    }

    checkIfBitten();

    // Serial << currentDistance << endl;  // Print out distance
  }

  else if (team == zombie)  // Zombie Team
  {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == "Human") {
        // Human close
        currentDistance = abs(WiFi.RSSI(i));
        showDistance(currentDistance, zombieColour);
      }

      if (currentDistance >= maxDistance) {
        allOff();
      }
    }
  }
}

void checkIfBitten() {
  // Getting Bitten
  if (currentDistance < bitingDistance) {
    // Been caught so start a timer
    if (!beenCaught) {
      timeCaught = millis();
      beenCaught = true;
    }

    timeNow = millis() - timeCaught;
    Serial << "Gonna get turned" << endl;

    // Turned
    if (timeNow >= timeTillTurned) {
      Serial << "Turned" << endl;

      for (int i = 0; i < 5; i++) {
        setAllLEDs(zombieColour);
        delay(500);
        allOff();
        delay(500);
      }

      team = zombie;
      startWiFi();
    }
  }

  // Not getting bitten
  else {
    timeCaught = 0;
    beenCaught = false;
  }
}