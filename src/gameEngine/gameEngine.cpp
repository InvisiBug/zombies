// #include "gameEngine.h"

// #include "Arduino.h"
// #include "Streaming.h"
// using namespace std;

// GameEngine::GameEngine(int totalLEDs, CRGB *currentLED, int interval) {
//   this->totalLEDs = totalLEDs;
//   this->currentLED = currentLED;
//   this->interval = interval;

//   colour = random(255);
// }

// void GameEngine::begin() {
//   initialInterval = interval;
// }

// //////////////////////////////////////////////////////////////////////////////
// //
// //  ######
// //  #     # #    # #    #
// //  #     # #    # ##   #
// //  ######  #    # # #  #
// //  #   #   #    # #  # #
// //  #    #  #    # #   ##
// //  #     #  ####  #    #
// //
// //////////////////////////////////////////////////////////////////////////////
// void GameEngine::run() {
//   currentMillis = millis();

//   if (currentMillis - lastMillis >= interval) {
//     lastMillis = currentMillis;

//     if ((currentStep < totalSteps)) {
//       for (int i = 0; i < totalLEDs; i++) {
//         currentLED[i].setHue(currentStep - i * (255 / totalLEDs));
//       }

//       FastLED.show();
//       currentStep++;
//     } else
//       currentStep = 0;
//   }
// }

// //////////////////////////////////////////////////////////////////////////////
// //
// // ###
// //  #  #    # ##### ###### #####  #    #   ##   #
// //  #  ##   #   #   #      #    # #    #  #  #  #
// //  #  # #  #   #   #####  #    # #    # #    # #
// //  #  #  # #   #   #      #####  #    # ###### #
// //  #  #   ##   #   #      #   #   #  #  #    # #
// // ### #    #   #   ###### #    #   ##   #    # ######
// //
// //////////////////////////////////////////////////////////////////////////////
