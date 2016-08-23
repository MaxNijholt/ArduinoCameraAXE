////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Donald Kelley (Copyright 2011, 2012)
// Modified by Maurice Ribble
// 2014-09 modified by D. M. Lenardon for addition of Bulb Timer delay and minor cleanup
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_STACKERMENU
////////////////////////////////////////
// Function: stackerMenu - Handles menuing for focusing rail photo stacking
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void stackerMenu()
{
  byte button     = BUTTON_NONE;
  byte maxChars   = 8;  // Max number of characters in edit fields
  byte yAdj = 0;
  int totDist;
  int units = eepromReadInt(EEPROM_STACKERMOTOR_UNITS);
  int stepsPerUnit = eepromReadInt(EEPROM_STACKERMOTOR_STEPSPERUNIT);

  if (g_mPos < 6)
    yAdj = 0;
  else if(g_mPos >= 8)
    yAdj = 27;
  else if(g_mPos >= 7)
    yAdj = 18;
  else if(g_mPos >= 6)
    yAdj = 9;

  printMenuTitle(_tx_05_stackerMenu,  maxChars, 0);
  if(g_mPos < 6) printMenuName(_tx_05_motorLoc,  0, yAdj);
  if(g_mPos < 7) printMenuName(_tx_05_picsStack, 1, yAdj);
  if(g_mPos < 8) printMenuName(_tx_05_motorDir,  2, yAdj);
  printMenuName(_tx_05_distUnit,  3, yAdj);
  printMenuName(_tx_05_stepsUnit, 4, yAdj);
  if(units == INCHES)
  {
    totDist = eepromReadInt(EEPROM_STACKERMOTOR_DISTANCE) * stepsPerUnit * .001;
  }
  else
  {
    totDist = eepromReadInt(EEPROM_STACKERMOTOR_DISTANCE) * stepsPerUnit * .01;
  }

  printMenuName(_tx_05_totalDist, 5, yAdj);
  if(g_mPos >= 6) printMenuName(_tx_05_settle, 6, yAdj);
  if(g_mPos >= 7) printMenuName(_tx_05_speedStepsSec, 7, yAdj);
  if(g_mPos >= 8) printMenuName(_tx_05_bulbSec, 8, yAdj);
 
  // Draw the changable values in this menu (only one can be selected at a time)
  if(g_mPos < 6) button |= printMenuSetString(maxChars,      0, g_mPos == 0, EEPROM_STACKERMOTOR_LOCATION,    8, 2, _tx_05_locationList, yAdj);
  if(g_mPos < 7) button |= printMenuSetNumber(maxChars,   1, g_mPos == 1, EEPROM_STACKERMOTOR_TOTPHOTOS,   2, 0, 9, yAdj);
  if(g_mPos < 8) button |= printMenuSetString(maxChars,      2, g_mPos == 2, EEPROM_STACKERMOTOR_DIR,         7, 2, _tx_05_dirList, yAdj);
  button |= printMenuSetString(maxChars,      3, g_mPos == 3, EEPROM_STACKERMOTOR_UNITS,       6, 2, _tx_05_unitsList, yAdj);
  button |= printMenuSetNumber(maxChars,   4, g_mPos == 4, EEPROM_STACKERMOTOR_STEPSPERUNIT,4, 0, 9, yAdj);
  if(units == INCHES)
    button |= printMenuSetNumber(maxChars, 5, g_mPos == 5, EEPROM_STACKERMOTOR_DISTANCE,   1, 3, 9, yAdj);
  else
    button |= printMenuSetNumber(maxChars, 5, g_mPos == 5, EEPROM_STACKERMOTOR_DISTANCE,   2, 2, 2, yAdj);
  if(g_mPos >= 6) button |= printMenuSetNumber(maxChars,   6, g_mPos == 6, EEPROM_STACKERMOTOR_SETTLETIME,  4, 0, 9, yAdj);
  if(g_mPos >= 7) button |= printMenuSetNumber(maxChars,   7, g_mPos == 7, EEPROM_STACKERMOTOR_SPEED,  4, 0, 9, yAdj);
  if(g_mPos >= 8) button |= printMenuSetNumber(maxChars,   8, g_mPos == 8, EEPROM_STACKERMOTOR_BULBSEC,  3, 1, 9, yAdj);

  menuProcessButton(button);
  menuScroll(button, 9);
}

void helpPrintStacker(int photoCnt, int totalPhotos)
{
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_05_stackerPhotoMode, 0xFF, 0);
    printMenuName(_tx_05_photosTaken, 0, 0);
    printMenuName(_tx_05_pressActivateToExit, 5, 0);
    g_dogm.setXY(88, 46);
    g_dogm.print(photoCnt);
    g_dogm.print('/');
    g_dogm.print(totalPhotos);
    if(photoCnt == totalPhotos)
    {
      printMenuName(_tx_05_photoStackComplete,2, 0);
    }
  } 
  while( g_dogm.next() );
}

////////////////////////////////////////
// Function: stackerFunc - Controls the stepper motors
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void stackerFunc()
{
  int mMotor = eepromReadInt(EEPROM_STACKERMOTOR_LOCATION); //which motor
  int mUnits = eepromReadInt(EEPROM_STACKERMOTOR_UNITS);    //inches or cm
  int mSpeed = eepromReadInt(EEPROM_STACKERMOTOR_SPEED);    //speed of motor(steps/sec)
  int mDir   = eepromReadInt(EEPROM_STACKERMOTOR_DIR);      //direction of motor
  int mTotPhotos    = eepromReadInt(EEPROM_STACKERMOTOR_TOTPHOTOS);    //total # of photos in stack
  int mSettleTime   = eepromReadInt(EEPROM_STACKERMOTOR_SETTLETIME);   //delay time after movement
  int mStepsPerUnit = eepromReadInt(EEPROM_STACKERMOTOR_STEPSPERUNIT); //steps per inch or cm
  int mTotDistance  = eepromReadInt(EEPROM_STACKERMOTOR_DISTANCE);     //total distance to move
  int photos;           //used to count photos taken
  int totSteps;         //total number of steps for total distance
  int mSteps;           //number of steps between photos
  int totDistTraveled;  //keep track of total steps taken
  int steps;            //mSteps + any lost steps
  int ms_shutterHold;   // milliseconds to hold the shutter and focus pins

  
  setSensorPowerPin(mMotor, LOW);
  setSensorPinModes(mMotor, OUTPUT, OUTPUT);

  attachInterrupt(0, activeButtonISR, LOW);

  //compute the total number of steps
  if(mUnits == INCHES)
  	totSteps = mTotDistance * .001 * mStepsPerUnit;
  else
  	totSteps = mTotDistance * .01 * mStepsPerUnit;

  // if x photos then x-1 movements
  mTotPhotos--;
  //computes the number of steps between photos
  mSteps = totSteps/mTotPhotos;
  //computes how many steps would be lost
  int lostSteps = totSteps % mTotPhotos; 

  photos = 0;
  totDistTraveled = 0;
// BULBSEC is in 1/10 sec units and shutterhold must be in milliseconds
// Use 400 ms as a minimum to insure camera/flash triggering
  ms_shutterHold = max( (eepromReadInt(EEPROM_STACKERMOTOR_BULBSEC) * 100), 400);

  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_05_stackerPhotoMode, 0xFF, 0);
    printMenuName(_tx_05_pressActivateToExit, 5, 0);
  } 
  while( g_dogm.next() );

  while(g_menuMode == MENUMODE_PHOTO)
  { // use <= to pick up the last photo
    while(photos <= mTotPhotos)
    {
      //take picture
      setDevicePins(0, HIGH, HIGH);
      setDevicePins(1, HIGH, HIGH);
      delay(ms_shutterHold);
      setDevicePins(0, LOW, LOW);
      setDevicePins(1, LOW, LOW);

      //update display
      helpPrintStacker(++photos, mTotPhotos+1);
      
      //if all pictures taken, get out of loop
      if(photos > mTotPhotos) break;

      //move camera to new position
      steps = mSteps + (lostSteps-- > 0?1:0);  //add in a lost step if any
      
      runStackerMotor(mMotor, mSpeed, mDir, steps);  //move motor
      totDistTraveled += steps;        //keep track of total steps
      
      //wait after move until next photo
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

