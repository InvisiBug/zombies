////////////////////////////////////////////////////////////////////////
//  Matthew Kavanagh
//
//  Zombies
//  27/02/2019
//  *********
////////////////////////////////////////////////////////////////////////
//
//  #####
// #     # #####   ##   #####  ##### #    # #####
// #         #    #  #  #    #   #   #    # #    #
//  #####    #   #    # #    #   #   #    # #    #
//       #   #   ###### #####    #   #    # #####
// #     #   #   #    # #   #    #   #    # #
//  #####    #   #    # #    #   #    ####  #
//
////////////////////////////////////////////////////////////////////////
/*
 * Started by the bottom button click when in the lobby
 */
void startWiFi() {
  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials
  delay(100);           // Added to try and prevent crashing (Remove if not possible)

  switch (team) {
    case human:
      Serial << "Player is a human" << endl;

      char humanssid[25];
      snprintf(humanssid, 25, "Human-%06X", ESP.getChipId());
      WiFi.softAP(humanssid, NULL, wifiChannel);
      break;

    case zombie:
      // Create a zombie wifi network using the chip id, can be used later to track individual zombies
      Serial << "Player is a zombie" << endl;

      char zombiessid[25];
      snprintf(zombiessid, 25, "Zombie-%06X", ESP.getChipId());
      WiFi.softAP(zombiessid, NULL, wifiChannel);
      break;
  }
}

////////////////////////////////////////////////////////////////////////
//
// #       ####### ######
// #       #       #     #  ####
// #       #       #     # #
// #       #####   #     #  ####
// #       #       #     #      #
// #       #       #     # #    #
// ####### ####### ######   ####
//
////////////////////////////////////////////////////////////////////////
// Sometimes doesnt actually set all LEDs, dunno why
void setAllLEDs(int colour) {
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = colour;
  }
  FastLED.show();
}

void showDistance(int currentDistance, int colour) {
  int indicatorLevel = map(currentDistance, maxDistance, bitingDistance, 0, totalLEDs);
  indicatorLevel = constrain(indicatorLevel, 0, totalLEDs);  // prevents crashing due to trying to show more than there actually are

  for (int i = 0; i < indicatorLevel; i++) {
    currentLED[i] = colour;
  }
  FastLED.show();

  for (int i = totalLEDs; i > indicatorLevel; i--) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}

void allOff() {
  for (int i = 0; i < totalLEDs; i++) {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}
