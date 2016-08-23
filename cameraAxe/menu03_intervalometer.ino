////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_INTERVALOMETERMENU
////////////////////////////////////////
// Function: intervalometerMenu - Handles menuing for the intervalometer
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void intervalometerMenu()
{
  byte button               = BUTTON_NONE;
  byte maxChars             = 8;  // Max number of characters in edit fields
  const byte yAdj           = 0;
  
  printMenuTitle(_tx_03_intervalometerMenu, maxChars, yAdj);
  printMenuName(_tx_03_startDelay,   0, yAdj);
  printMenuName(_tx_03_interval,     1, yAdj);
  printMenuName(_tx_03_shots,        2, yAdj);
  printMenuName(_tx_03_bulb,         3, yAdj);
  printMenuName(_tx_03_hdrStops,     4, yAdj);
  printMenuName(_tx_03_mirrorLockup, 5, yAdj);

  g_dogm.setXY(88, 46);  // Colons between hours/minutes/seconds
  g_dogm.print(getStr(_tx_03_timeColon));
  g_dogm.setXY(88, 37);
  g_dogm.print(getStr(_tx_03_timeColon));
  
  button |= printMenuSetNumber(maxChars-0, 0, g_mPos == 0, EEPROM_INTERVALOMETER_DELAY_HOUR,   2, 0 , 9, 0);
  button |= printMenuSetNumber(maxChars-3, 0, g_mPos == 1, EEPROM_INTERVALOMETER_DELAY_MIN,    2, 0 , 5, 0);
  button |= printMenuSetNumber(maxChars-6, 0, g_mPos == 2, EEPROM_INTERVALOMETER_DELAY_SEC,    2, 0 , 5, 0);
  button |= printMenuSetNumber(maxChars-0, 1, g_mPos == 3, EEPROM_INTERVALOMETER_HOUR,         2, 0 , 9, 0);
  button |= printMenuSetNumber(maxChars-3, 1, g_mPos == 4, EEPROM_INTERVALOMETER_MIN,          2, 0 , 5, 0);
  button |= printMenuSetNumber(maxChars-6, 1, g_mPos == 5, EEPROM_INTERVALOMETER_SEC,          2, 0 , 5, 0);
  button |= printMenuSetNumber(maxChars,   2, g_mPos == 6, EEPROM_INTERVALOMETER_SHOTS,        4, 0 , 9, 0);
  button |= printMenuSetNumber(maxChars,   3, g_mPos == 7, EEPROM_INTERVALOMETER_BULB,         3, 1 , 9, 0);
  button |= printMenuSetNumber(maxChars,   4, g_mPos == 8, EEPROM_INTERVALOMETER_HDRSTOPS,     1, 0 , 9, 0);
  button |= printMenuSetString(maxChars,   5, g_mPos == 9, EEPROM_INTERVALOMETER_MIRRORLOCKUP, 3, 2, _tx_03_noYesList, 0);
  menuProcessButton(button);
  
  if (!g_editVal)
  {
    switch (button)
    {
      case BUTTON_UP:
        {
          if (g_mPos <= 2)
            g_mPos = 9;
          else if (g_mPos <=6)
            g_mPos = max(0, g_mPos-3);
          else
            --g_mPos;
        }
        break;
      case BUTTON_DOWN:
        {
          if (g_mPos == 9)
            g_mPos = 0;
          else if (g_mPos <= 4)
            g_mPos = min(6, g_mPos+3);
          else
            ++g_mPos;
        }
        break;
      case BUTTON_LEFT:
        {
          if (g_mPos == 0)
            g_mPos = 2;
          else if (g_mPos <= 2)
            --g_mPos;
          else if (g_mPos == 3)
            g_mPos = 5;
          else if (g_mPos <= 5)
            --g_mPos;
        }
        break;
      case BUTTON_RIGHT:
        {
          if (g_mPos == 2)
            g_mPos = 0;
          else if (g_mPos <= 1)
            ++g_mPos;
          else if (g_mPos == 5)
            g_mPos = 3;
          else if (g_mPos <= 4)
            ++g_mPos;
        }
        break;
    }
  }  
}

////////////////////////////////////////
// Function: helpDrawIntervalometer - Draws intervalometer menu to LCD
// Parameters:
//   delayHours   - Number of hours before we start taking shots
//   delayMinutes - Number of minutes before we start taking shots
//   delaySeconds - Number of seconds before we start taking shots
//   hours        - Number of hours between shots
//   minutes      - Number of minutes between shots
//   seconds      - Number of seconds between shots
//   shots        - Number of shots
//   bulb         - Time in seconds for exposure
//   hdrStops     - Number of bracketed stops for HDR images
// Returns:
//   None
////////////////////////////////////////
void helpDrawIntervalometer(int delayHours, int delayMinutes, int delaySeconds, int hours, int minutes, int seconds, int shots, int bulb, int hdrStops)
{
  const byte yAdj           = 0;  
  
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_03_intervalometerPhotoMode, 0xFF, yAdj);
    printMenuName(_tx_03_startDelay,   0, yAdj);
    printMenuName(_tx_03_interval,     1, yAdj);
    printMenuName(_tx_03_shots,        2, yAdj);
    printMenuName(_tx_03_bulb,         3, yAdj);
    printMenuName(_tx_03_hdrStops,     4, yAdj);

    // Draw constant lines
    g_dogm.setVLine(85, 10, 54);
    g_dogm.setBox(0, 54, 127, 55);

    // Draw the changable values in this menu (only one can be selected at a time)
    lcdPrintZeros(87, 46, decimalToBcd(delayHours), 2, 0);
    lcdPrintZeros(102, 46, decimalToBcd(delayMinutes), 2, 0);
    lcdPrintZeros(117, 46, decimalToBcd(delaySeconds), 2, 0);
    lcdPrintZeros(87, 37, decimalToBcd(hours), 2, 0);
    lcdPrintZeros(102, 37, decimalToBcd(minutes), 2, 0);
    lcdPrintZeros(117, 37, decimalToBcd(seconds), 2, 0);
    lcdPrintZeros(87, 28, decimalToBcd(shots), 4, 0);
    lcdPrintZeros(87, 19, decimalToBcd(bulb), 3, 1);
    lcdPrintZeros(87, 10, decimalToBcd(hdrStops), 1, 0);

    g_dogm.setXY(10, 0);
    g_dogm.print(getStr(_tx_03_pressActivateToExit));
  }
  while( g_dogm.next() );
}

////////////////////////////////////////
// Function: intervalometerFunc - Handles triggering for intervalometer
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void intervalometerFunc()
{
  int delayHours          = eepromReadInt(EEPROM_INTERVALOMETER_DELAY_HOUR);
  int delayMinutes        = eepromReadInt(EEPROM_INTERVALOMETER_DELAY_MIN);
  int delaySeconds        = eepromReadInt(EEPROM_INTERVALOMETER_DELAY_SEC);
  int hours               = eepromReadInt(EEPROM_INTERVALOMETER_HOUR);
  int minutes             = eepromReadInt(EEPROM_INTERVALOMETER_MIN);
  int seconds             = eepromReadInt(EEPROM_INTERVALOMETER_SEC);
  int shots               = eepromReadInt(EEPROM_INTERVALOMETER_SHOTS);
  int bulb                = eepromReadInt(EEPROM_INTERVALOMETER_BULB);
  int hdrStops            = eepromReadInt(EEPROM_INTERVALOMETER_HDRSTOPS);
  byte mirrorLockup       = eepromReadInt(EEPROM_INTERVALOMETER_MIRRORLOCKUP);
  int origHours           = eepromReadInt(EEPROM_INTERVALOMETER_HOUR);  
  int origMinutes         = eepromReadInt(EEPROM_INTERVALOMETER_MIN);
  int origSeconds         = eepromReadInt(EEPROM_INTERVALOMETER_SEC);
  
  byte startingDelay      = ((delayHours != 0) || (delayMinutes != 0) || (delaySeconds != 0)) ? 1 : 0;
  byte tHdrShots          = 0;
  byte triggering         = 0;
  byte firstPass          = 1;
  unsigned long timer     = millis();
  unsigned long offTimer  = 0;
  unsigned long offLockup = 0;

  attachInterrupt(0, activeButtonISR, LOW);
  helpDrawIntervalometer(delayHours, delayMinutes, delaySeconds, hours, minutes, seconds, shots, bulb, hdrStops);
 
  while(g_menuMode == MENUMODE_PHOTO)
  {
    unsigned long msTillOff;
   
    while ((millis() < timer) && (g_menuMode == MENUMODE_PHOTO))  // This is the wait loop
    {
      {// Manual bulb ramping or bramping
        byte button = detectButtonPress(true);
        if (button == BUTTON_UP)
        {
          bulb = min(bulb + 1, 9999);
          eepromWriteInt(EEPROM_INTERVALOMETER_BULB, bulb);
        }
        else if (button == BUTTON_DOWN)
        {
          bulb = max(bulb - 1, 0);
          eepromWriteInt(EEPROM_INTERVALOMETER_BULB, bulb);
        }
      }
      
      if (offLockup && (millis() >= offLockup))
      {
        msTillOff = ((unsigned long)bulb << tHdrShots)*100;
        offLockup = 0;
        
        setDevicePins(0, LOW, LOW);
        setDevicePins(1, LOW, LOW);
        delay(500);  // 0.5 second - Second part of mirror lockup for total of 2 seconds

        setDevicePins(0, HIGH, HIGH);
        setDevicePins(1, HIGH, HIGH);
        offTimer = millis() + msTillOff;
      }
      
      if (!offLockup && offTimer && (millis() >= offTimer))  // Handles turning off camera
      {
        int forceDelay = 0;
        
        if ((offTimer >= timer) || ((timer-offTimer) < 500))  // Handle case when bulb time goes past next internval time
          forceDelay = 500;
  
        offTimer = 0;
        ++tHdrShots;

        if (tHdrShots > hdrStops)  // Turn off trigger if all hdr shots taken (ie ready for next shot)
        {
          triggering = 0;
          if (shots == 1) // Exit when shots are done
            g_menuMode = MENUMODE_MENU;
        }

        setDevicePins(0, LOW, LOW);
        setDevicePins(1, LOW, LOW);
        delay(forceDelay);
      }
    }

    timer += 1000;

    if (startingDelay) // Start delay
    {
      if (firstPass == 0)
        delaySeconds--;
      else
        firstPass = 0;
      
      if ((delaySeconds == 0) && (delayMinutes == 0) && (delayHours == 0))
      {
        startingDelay = 0;
        triggering = 1;
      }
      else if ((delaySeconds == -1) && (delayMinutes == 0))
      {
        delaySeconds = 59;
        delayMinutes = 59;
        delayHours--;
      }
      else if (delaySeconds == -1)
      {
        delaySeconds = 59;
        delayMinutes--;
      }
    }
    else // Normal interval delay
    {
      if (firstPass == 0)
        seconds--;
      else
        firstPass = 0;

      if ((seconds == 0) && (minutes == 0) && (hours == 0))
      {
        triggering = 1;
      }
      else if ((seconds == -1) && (minutes == 0))
      {
        seconds = 59;
        minutes = 59;
        hours--;
      }
      else if (seconds == -1)
      {
        seconds = 59;
        minutes--;
      }
    }
    
    if (triggering && (offTimer == 0)) // Triggering
    {
      // trigger camera
      setDevicePins(0, HIGH, HIGH);
      setDevicePins(1, HIGH, HIGH);

      if ((seconds == 0) && (minutes == 0) && (hours == 0))
      {
        seconds = origSeconds;
        minutes = origMinutes;
        hours = origHours;
      }        

      if (tHdrShots > hdrStops)
      {
        shots = max (-1, shots-1); // Prevent shots from wrapping around
        tHdrShots = 0;
      }

      if (mirrorLockup)
      {
        offLockup = millis() + 1500; // 1.5 second mirror lockup time (another 0.5 seconds applied above for total 2 second)
        offTimer = offLockup;
      }
      else
      {
        msTillOff = ((unsigned long)bulb << tHdrShots)*100;
        offTimer = millis() + msTillOff;
      }
    }

    if (hours < 0)
    {
      seconds = 0;
      minutes = 0;
      hours = 0;
      helpDrawIntervalometer(delayHours, delayMinutes, delaySeconds, hours, minutes, seconds, max(0, shots), bulb, hdrStops);
      seconds = 1;
    }
    else
      helpDrawIntervalometer(delayHours, delayMinutes, delaySeconds, hours, minutes, seconds, max(0, shots), bulb, hdrStops);
  }

  buttonDebounce(10); // 10 ms debounce
}
#endif

