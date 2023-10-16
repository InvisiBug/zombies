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
void start(char* team)
{
  allOff();
  WiFi.mode(WIFI_OFF);  // Clears the last wifi credentials

  delay(100); // Added to try and prevent crashing (Remove if not possible)

  WiFi.mode(WIFI_AP_STA);  // Wifi Modes (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA)
  WiFi.softAP(team, NULL, wifiChannel);

  Serial << "Player is a " << team << endl;  
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
void countDownAnimation() // Add random selection
{
  int colour;
  if     (digitalRead(humanPin))  colour = humanColour; // Human animation
  else if(digitalRead(zombiePin)) colour = zombieColour; // Zombie Animation
  // else if(team == "Random") colour = randomColour; // Random animation

  for(int i = 0; i < totalLEDs; i ++)
  {
    currentLED[i] = colour;
  }
  FastLED.show();

  delay(150);

  for(int i = 0; i < totalLEDs; i ++)
  {
    currentLED[i] = 0x000000;
  }
  FastLED.show();

  delay(150);
}

void showTimeLeft()
{
  int indicatorLevel  = map(gameTimeLeft, 0, totalGameTime - 100, 0, totalLEDs);

  showDistance(indicatorLevel, timeColour);
  delay(500);

  allOff();
}

void showDistance(int indicatorLevel, int colour)
{
  indicatorLevel = constrain(indicatorLevel, 0, totalLEDs); // prevents crashing due to trying to show more than there actually are

  for(int i = 0; i < indicatorLevel; i ++)
  {
    currentLED[i] = colour;
  }
  FastLED.show();

  for(int i = totalLEDs; i > indicatorLevel; i --)
  {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}

void allOff()
{
  for(int i = 0; i < totalLEDs; i ++)
  {
    currentLED[i] = 0x000000;
  }
  FastLED.show();
}

////////////////////////////////////////////////////////////////////////
//
// ######
// #     # #    # ##### #####  ####  #    #
// #     # #    #   #     #   #    # ##   #
// ######  #    #   #     #   #    # # #  #
// #     # #    #   #     #   #    # #  # #
// #     # #    #   #     #   #    # #   ##
// ######   ####    #     #    ####  #    #
//
////////////////////////////////////////////////////////////////////////
void click()
{
  Serial << "Click" << endl; 

  if(digitalRead(gamePin)) 
  {
    if(!gameReady) gameReady = true;
  }

  if((digitalRead(gamePin)) && (gameRunning))
  {
    showTimeLeft();
  }

  if((digitalRead(gamePin) && gameFinished))
  {
    resetGame();
  }
}

void longPress()
{
  // Serial << "Long Press" << endl;
  started = false;
}

void doubleClick()
{  
  // Serial << "Double Click" << endl;

  // if     (mode == "Game")    mode = "Ambient";
  // else if(mode == "Ambient") mode = "Game";

  // Serial << "Switching to " << mode << endl;
}

////////////////////////////////////////////////////////////////////////
//
// #######                               
//    #    # #    # ###### #####   ####  
//    #    # ##  ## #      #    # #      
//    #    # # ## # #####  #    #  ####  
//    #    # #    # #      #####       # 
//    #    # #    # #      #   #  #    # 
//    #    # #    # ###### #    #  ####  
//
//////////////////////////////////////////////////////////////////////// 
bool gameTimeRemaining()
{
  gameTimeLeft = totalGameTime - (millis() - timeGameStarted);

  printTimeRemaining(gameTimeLeft);

  if(gameTimeLeft > 0) return true;
  else return false;
}

bool preStartCountDownFinished()
{
  preStartTimeLeft = totalPreStartTime - (millis() - timeCountDownStarted);

  printTimeRemaining(preStartTimeLeft);

  if(preStartTimeLeft < 0) return true;
  else return false;
}

////////////////////////////////////////////////////////////////////////
//
//  #     #                 
//  ##   ## #  ####   ####  
//  # # # # # #      #    # 
//  #  #  # #  ####  #      
//  #     # #      # #      
//  #     # # #    # #    # 
//  #     # #  ####   ####  
//
//////////////////////////////////////////////////////////////////////// 
void printTimeRemaining(int time)
{
  int minsLeft    = (time  % (1000 * 60 * 60)) / (1000 * 60);
  int secondsLeft = ((time % (1000 * 60 * 60)) % (1000 * 60)) / 1000;
  Serial << "Remaining Time " << minsLeft  << ":" << secondsLeft << endl;
}

void resetGame()
{
  allOff();

  gameTimeLeft     = totalGameTime; // 45 mins
  preStartTimeLeft = totalPreStartTime;

  timeGameStarted      = 0;
  timeCountDownStarted = 0;

  gameReady            = false;
  gameRunning          = false;
  started              = false;
  restartGame          = false;
  countDownFinished    = false;
  countDownRunning     = false;
  gameFinished         = false;
  preGameLobbyCounter  = totalLEDs - 1;
  postGameLobbyCounter = totalLEDs - 1;
}