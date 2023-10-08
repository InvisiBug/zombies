////////////////////////////////////////////////////////////////////////
//  Matthew Kavanagh
//
//  Zombies
//  27/02/2019
//  *********
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
  crissCross.run(50);

  int n = WiFi.scanNetworks(false, false, wifiChannel);

  if (team == "Human")  // Human Team
  {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == "Zombie")  // Zombie close
      {
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

    Serial << currentDistance << endl;  // Print out distance
  }

  else if (team == "Zombie")  // Zombie Team
  {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == "Human")  // Human close
      {
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
  if (currentDistance < bitingDistance)  // Getting Bitten
  {
    if (!beenCaught) {
      timeCaught = millis();
      beenCaught = true;
    }

    timeNow = millis() - timeCaught;
    Serial << "Gonna get turned" << endl;

    if (timeNow >= timeTillTurned)  // Turned
    {
      Serial << "Turned" << endl;

      team = "Zombie";
      start(team);
    }
  }

  else  // Not getting bitten
  {
    timeCaught = 0;
    beenCaught = false;
  }
}