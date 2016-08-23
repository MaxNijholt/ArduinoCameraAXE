////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Donald Kelley (Copyright 2011, 2012)
// Modified by Maurice Ribble
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_JOGMENU
////////////////////////////////////////
// Function: panoMenu - Handles menuing for focusing rail photo stacking
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void jogMenu()
{
  byte button     = BUTTON_NONE;
  byte maxChars   = 8;  // Max number of characters in edit fields
  const byte yAdj = 0;

  printMenuTitle(_tx_07_jogMotorMenu,  maxChars, yAdj);
  printMenuName(_tx_07_motorLocation, 0, yAdj);
  printMenuName(_tx_07_stepsJogLR,    1, yAdj);
  printMenuName(_tx_07_stepsJogUD,    2, yAdj);
  printMenuName(_tx_07_speed,         3, yAdj);
 
  // Draw the changable values in this menu (only one can be selected at a time)
  button |= printMenuSetString(maxChars,    0, g_mPos == 0, EEPROM_MOTORJOG_LOCATION,     8, 2, _tx_07_locationList, yAdj);
  button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_MOTORJOG_BSTEPSPERJOG, 4, 0, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 2, g_mPos == 2, EEPROM_MOTORJOG_LSTEPSPERJOG, 4, 0, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 3, g_mPos == 3, EEPROM_MOTORJOG_STEPSPERSEC,  4, 0, 9, yAdj);

  menuProcessButton(button);
  menuScroll(button, 4);
}

void helpPrintJog(int dir, int stepsPerJog, int steps, boolean waitForRelease)
{
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_07_jogMotorMode, 0xFF, 0);
    printMenuName(_tx_07_direction,          0, 0);
    printMenuName(_tx_07_jogMode,           1, 0);
    printMenuName(_tx_07_stepsJog,          2, 0);
    printMenuName(_tx_07_totSteps,          3, 0);
    printMenuName(_tx_07_pressActivateToExit, 5, 0);
    g_dogm.setXY(78, 46);
    if(dir)
    	g_dogm.print(getStr(_tx_07_forward));
    else
    	g_dogm.print(getStr(_tx_07_backward));
    g_dogm.setXY(78, 37);
    if(waitForRelease)
      	g_dogm.print(getStr(_tx_07_single));
    else
    	g_dogm.print(getStr(_tx_07_continuous));
    g_dogm.setXY(78, 28);
    g_dogm.print(stepsPerJog);
    g_dogm.setXY(78, 19);
    g_dogm.print(steps);
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
void jogFunc()
{
  int mMotor = eepromReadInt(EEPROM_MOTORJOG_LOCATION); //which motor
  int mSpeed = eepromReadInt(EEPROM_MOTORJOG_STEPSPERSEC); //speed to move
  int bstepsPerJog = eepromReadInt(EEPROM_MOTORJOG_BSTEPSPERJOG); //steps for button press
  int lstepsPerJog = eepromReadInt(EEPROM_MOTORJOG_LSTEPSPERJOG); //steps for button press
  int totSteps = 0;
  byte button = BUTTON_NONE;
  byte mDir = 0;
  int stepsPerJog = 0;
  boolean waitForRelease = true;
  
  setSensorPowerPin(mMotor, LOW);
  setSensorPinModes(mMotor, OUTPUT, OUTPUT);

  attachInterrupt(0, activeButtonISR, LOW);
  
  helpPrintJog(mDir, stepsPerJog, totSteps, waitForRelease);
  
  while(g_menuMode == MENUMODE_PHOTO)
  {
      button = detectButtonPress(waitForRelease);
      //buttonDebounce(2);
      switch (button)
      {
      case BUTTON_RIGHT:
        {
          mDir = 0;
          stepsPerJog = bstepsPerJog;
          runStackerMotor(mMotor, mSpeed, mDir, stepsPerJog);  //move motor
	  totSteps += stepsPerJog;
          //helpPrintJog(mDir, stepsPerJog, totSteps, waitForRelease);
          break;
        }
      case BUTTON_LEFT:
        {
          stepsPerJog = bstepsPerJog;
          mDir = 1;
          runStackerMotor(mMotor, mSpeed, mDir, stepsPerJog);  //move motor
	  totSteps -= stepsPerJog;
          //helpPrintJog(mDir, stepsPerJog, totSteps);
          break;
        }
        case BUTTON_UP:
        {
          stepsPerJog = lstepsPerJog;
          mDir = 0;
          runStackerMotor(mMotor, mSpeed, mDir, stepsPerJog);  //move motor
	  totSteps += stepsPerJog;
          //helpPrintJog(mDir, stepsPerJog, totSteps);
          break;
        }
        
      case BUTTON_DOWN:
        {
          stepsPerJog = lstepsPerJog;
          mDir = 1;
          runStackerMotor(mMotor, mSpeed, mDir, stepsPerJog);  //move motor
	  totSteps -= stepsPerJog;
          //helpPrintJog(mDir, stepsPerJog, totSteps);
          break;
        }
      case BUTTON_SELECT:
        {
          waitForRelease = !waitForRelease;
          buttonDebounce(10);
          break;
        }
      }
      helpPrintJog(mDir, stepsPerJog, totSteps, waitForRelease);
  }

  buttonDebounce(10); // 10 ms debounce
}
#endif

