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
void runGameWithoutBuffer() {
  int n = WiFi.scanNetworks(false, false, wifiChannel);

  if (team == human) {
    //* Maybe make it only display the closest zombie
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i).indexOf("Zombie") != -1) {
        int currentDistance = abs(WiFi.RSSI(i));

        Serial << currentDistance << endl;  // Print out distance

        showDistance(currentDistance, humanColour);

        if (currentDistance < bitingDistance) {
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
    }

    if (currentDistance >= maxDistance) {
      allOff();
    }

  }

  else if (team == zombie)  // Zombie Team
  {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i).indexOf("Human") != -1) {
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

void runGameWithBuffer() {
  int n = WiFi.scanNetworks(false, false, wifiChannel);

  // Calculate the average of the buffer array
  int bufferSum = 0;
  for (int i = 0; i < 5; i++) {
    bufferSum += buffer[i];
  }
  bufferAverage = bufferSum / 5;

  if (team == human) {
    currentDistance = maxDistance;

    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i) == "Zombie") {
        // Zombie close

        currentDistance = abs(WiFi.RSSI(i));

        // and save the new distance if it is within the threshold of the average

        showDistance(bufferAverage, humanColour);

        // Save the value regardless if the buffer is empty (bitingDistance + 1)
        // otherwise no values would get saved
        if (buffer[bufferPos] == bitingDistance + 1) {
          buffer[bufferPos] = currentDistance;
          bufferPos++;
        }

        int threshold = 10;

        // Save value if its within the threshold of the buffer average
        if ((currentDistance > bufferAverage - threshold) && (currentDistance < bufferAverage + threshold)) {
          Serial << "Buffer average: " << bufferAverage << " Save value : " << currentDistance << endl;

          // Add each new distance to the buffer array
          if (bufferPos < 5) {
            buffer[bufferPos] = currentDistance;
            bufferPos++;
          } else {
            bufferPos = 0;
          }

        } else {
          Serial << "Buffer average: " << bufferAverage << " Discard value: " << currentDistance << endl;
        }

        // Serial << "Buffer average: " << bufferAverage << endl;

        bufferSum = 0;

        // Serial << "Buffer: [";
        // for (int i = 0; i < 5; i++) {
        //   Serial << "[" << buffer[i] << "]";
        // }
        // Serial << "]" << endl;
      }
    }

    if (bufferAverage >= maxDistance) {
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
  if (bufferAverage < bitingDistance) {
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
