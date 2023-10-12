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
void game() {
  int n = WiFi.scanNetworks(false, false, wifiChannel);

  if (team == human) {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == "Zombie") {
        // Zombie close
        currentDistance = abs(WiFi.RSSI(i));
        int indicatorLevel = map(currentDistance, maxDistance, bitingDistance, 0, totalLEDs);
        indicatorLevel = constrain(indicatorLevel, 0, totalLEDs);  // prevents crashing due to trying to show more than there actually are

        showDistance(indicatorLevel, humanColour);

        Serial << "Current Distance: " << currentDistance << " "
               << "Indicator Level: " << indicatorLevel << endl;
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
        int indicatorLevel = map(currentDistance, maxDistance, bitingDistance, 0, totalLEDs);
        indicatorLevel = constrain(indicatorLevel, 0, totalLEDs);  // prevents crashing due to trying to show more than there actually are

        showDistance(indicatorLevel, zombieColour);

        Serial << "Current Distance: " << currentDistance << " "
               << "Indicator Level: " << indicatorLevel << endl;
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
    if (!beenCaught) {
      timeCaught = millis();
      beenCaught = true;
    }

    timeNow = millis() - timeCaught;
    Serial << "Gonna get turned" << endl;

    // Turned
    if (timeNow >= timeTillTurned) {
      Serial << "Turned" << endl;

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