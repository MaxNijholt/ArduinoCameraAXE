////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Donald Kelley (Copyright 2011, 2012)
// Modified by Maurice Ribble
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_PANORAMAMENU
////////////////////////////////////////
// Function: panoMenu - Handles menuing for focusing rail photo stacking
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void panoMenu()
{
  byte button     = BUTTON_NONE;
  byte maxChars   = 8;  // Max number of characters in edit fields
  byte yAdj = 0;

  if(g_mPos < 6)
    yAdj = 0;
  else if(g_mPos >= 7)
    yAdj = 18;
  else if(g_mPos >= 6)
    yAdj = 9;

  printMenuTitle(_tx_06_panoramaMenu,  maxChars, 0);
  if(g_mPos < 6)printMenuName(_tx_06_motorLoc,    0, yAdj);
  printMenuName(_tx_06_picsPano,        1, yAdj);
  printMenuName(_tx_06_motorDir,        2, yAdj);
  printMenuName(_tx_06_stepsRevolution, 3, yAdj);	//will always be an integer
  printMenuName(_tx_06_totDegrees,      4, yAdj);
  printMenuName(_tx_06_settle,          5, yAdj);
  if(g_mPos >= 6) printMenuName(_tx_06_speedStepsSec,   6, yAdj);
  if(g_mPos >= 7) printMenuName(_tx_06_bulbSec, 7, yAdj);
 
  // Draw the changable values in this menu (only one can be selected at a time)
  if(g_mPos < 6) button |= printMenuSetString(maxChars,    0, g_mPos == 0, EEPROM_PANOMOTOR_LOCATION,    8, 2, _tx_06_locationList, yAdj);
  button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_PANOMOTOR_TOTPHOTOS,   2, 0, 9, yAdj);
  button |= printMenuSetString(maxChars,    2, g_mPos == 2, EEPROM_PANOMOTOR_DIR,         7, 2, _tx_06_dirList, yAdj);
  button |= printMenuSetNumber(maxChars, 3, g_mPos == 3, EEPROM_PANOMOTOR_STEPSPERREV, 4, 0, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 4, g_mPos == 4, EEPROM_PANOMOTOR_TOTALDEGREES,3, 0, 3, yAdj);
  button |= printMenuSetNumber(maxChars, 5, g_mPos == 5, EEPROM_PANOMOTOR_SETTLETIME,  4, 0, 9, yAdj);
  if(g_mPos >= 6) button |= printMenuSetNumber(maxChars, 6, g_mPos == 6, EEPROM_PANOMOTOR_SPEED,  4, 0, 9, yAdj);
  if(g_mPos >= 7) button |= printMenuSetNumber(maxChars,   7, g_mPos == 7, EEPROM_PANOMOTOR_BULBSEC,  3, 1, 9, yAdj);
  
  menuProcessButton(button);
  menuScroll(button, 8);
}

void helpPrintPano(int photoCnt, int totalPhotos)
{
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_06_panoramaPhotoMode, 0xFF, 0);
    printMenuName(_tx_06_photosTaken, 0, 0);
    printMenuName(_tx_06_pressActivateToExit, 5, 0);
    g_dogm.setXY(88, 46);
    g_dogm.print(photoCnt);
    g_dogm.print('/');
    g_dogm.print(totalPhotos);
    if(photoCnt == totalPhotos)
    {
      printMenuName(_tx_06_panoramaComplete,2, 0);
    }
  } 
  while( g_dogm.next() );
}

////////////////////////////////////////
// Function: panoFunc - Controls the stepper motors
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void panoFunc()
{
  int mMotor = eepromReadInt(EEPROM_PANOMOTOR_LOCATION); //which motor
  int mSpeed = eepromReadInt(EEPROM_PANOMOTOR_SPEED);    //speed of motor
  int mDir   = eepromReadInt(EEPROM_PANOMOTOR_DIR);      //direction of motor
  int mTotPhotos    = eepromReadInt(EEPROM_PANOMOTOR_TOTPHOTOS);    //total # of photos in stack
  int mSettleTime   = eepromReadInt(EEPROM_PANOMOTOR_SETTLETIME);   //delay time after movement
  int mStepsPerRev  = eepromReadInt(EEPROM_PANOMOTOR_STEPSPERREV);  //steps per inch or cm
  int mTotDistance  = eepromReadInt(EEPROM_PANOMOTOR_TOTALDEGREES); //total distance to move
  int photos;           //used to count photos taken
  int totSteps;         //total number of steps for total distance
  int mSteps;           //number of steps between photos
  int totDistTraveled;  //keep track of total steps taken
  int steps;            //mSteps + any lost steps
  int mDegsPerStep;
  int ms_shutterHold;   // milliseconds to hold the shutter and focus pins
  //unsigned int dt;      //this is millis() + mSettleTIme

  setSensorPowerPin(mMotor, LOW);
  setSensorPinModes(mMotor, OUTPUT, OUTPUT);

  attachInterrupt(0, activeButtonISR, LOW);

  //compute the total number of steps (times 10 in case of fractions)
  mDegsPerStep = 3600 / mStepsPerRev;
  totSteps = mTotDistance*10 / mDegsPerStep;

  //computes the number of steps between photos
  mSteps = totSteps/mTotPhotos;
  //computes how many steps would be lost
  int lostSteps = totSteps % mTotPhotos; 

  photos = 0;
  totDistTraveled = 0;
  
// BULBSEC is in 1/10 sec units and shutterhold must be in milliseconds
// Use 400 ms as a minimum to insure camera/flash triggering
  ms_shutterHold = max( (eepromReadInt(EEPROM_PANOMOTOR_BULBSEC) * 100), 400);

  
  while(g_menuMode == MENUMODE_PHOTO)
  { // use <= to pick up the last photo
    while(photos < mTotPhotos)
    {
      //take picture
      setDevicePins(0, HIGH, HIGH);
      setDevicePins(1, HIGH, HIGH);
      delay(ms_shutterHold);
      setDevicePins(0, LOW, LOW);
      setDevicePins(1, LOW, LOW);

      //update display
      helpPrintPano(++photos, mTotPhotos);
      
      //if all pictures taken, get out of loop
      if(photos >= mTotPhotos) break;

      //move camera to new position
      steps = mSteps + (lostSteps-- > 0?1:0);  //add in a lost step if any
      
      runStackerMotor(mMotor, mSpeed, mDir, steps);  //move motor
      totDistTraveled += steps;        //keep track of total steps
      
      //dt = millis() + mSettleTime;
      //while(millis() < dt && g_menuMode == MENUMODE_PHOTO)
      //  ;
      delay(mSettleTime);
    }
    
    // return camera to the original position
    runStackerMotor(mMotor, mSpeed, !mDir, totDistTraveled);
    while(g_menuMode == MENUMODE_PHOTO)
      ;
  }

  buttonDebounce(10); // 10 ms debounce
}
#endif

