////////////////////////////////////////////////////////////////////////
//  Matthew Kavanagh 
// 
//  Zombies
//  27/02/2019
//  *********
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
void setupGame()
{
  // Serial << "Setup Game" << endl;
  if(gameReady) countDown();    // Players ready, begin countdown
  else          preGameLobby(); // Players not ready, pre game lobby
}

void countDown() // Pre start countdown
{
  // Serial << "Pre Start Countdown" << endl;
  if(countDownRunning) checkCountDownTime(); // Start count down
  else // Countdown starts, take current time  
  {
    countDownRunning     = true;
    timeCountDownStarted = millis();
  }
}

void checkCountDownTime()
{
  // Serial << "Check Count Down Time" << endl;
  if(preStartCountDownFinished()) runGame();
  else countDownAnimation(); // count down animation;
}


void runGame()
{
  if(gameRunning) checkGameTime(); // Game running
  else // Game starts, take current time  
  {
    gameRunning = true;
    timeGameStarted = millis();
  }
}

void checkGameTime()
{
  if(gameTimeRemaining()) game(); // Still time remaining, play game
  else endGame(); // Game finished
}

void endGame()
{
  if(restartGame) resetGame(); // Restarting game
  else endGameLobby(); // Game finished, final lobby
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
//////////////////////////////////////////////////////////////////////// 
void preGameLobby() // Add random selection
{
  int teamColour;
  if(digitalRead(humanPin))  
  {
    team = "Human";
    teamColour = humanColour; // Human animation
  }
  else if(digitalRead(zombiePin)) 
  {
    team = "Zombie";
    teamColour = zombieColour; // Zombie Animation
  }
  else
  {
    team = "";
    teamColour = 0x000000; // Zombie Animation
  }

  // else if(team == "Random") teamColour = randomColour; // Random animation

  for(int i = 0; i < totalLEDs; i ++)
  {
    currentLED[i] = teamColour;
  }

  if(preGameLobbyCounter >= 0)
  {
    currentLED[preGameLobbyCounter] = 0x000000;
    preGameLobbyCounter --;

    FastLED.show();
    delay(100);
  }

  else preGameLobbyCounter = totalLEDs - 1;
}

void endGameLobby()
{
  Serial << "End Game Lobby" << endl;

  gameFinished = true;

  int teamColour;
  if     (team == "Human")  teamColour = humanColour;  // Human animation
  else if(team == "Zombie") teamColour = zombieColour; // Zombie Animation

  for(int i = 0; i < totalLEDs; i ++)
  {
    currentLED[i] = 0x000000;
  }

  if(postGameLobbyCounter >= 0)
  {
    currentLED[postGameLobbyCounter] = teamColour;
    postGameLobbyCounter --;

    Serial << postGameLobbyCounter << endl;

    FastLED.show();
    delay(100);
  }

  else postGameLobbyCounter = totalLEDs - 1;
}