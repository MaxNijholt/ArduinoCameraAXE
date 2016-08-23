////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This mode uses uses two sensors to detect an object triggering sensor1 and then sensor2.  A set of photos will then be taken.
// Sensor1 must be triggered before sensor2 or photos are not taken.

#ifdef USE_ROLLERCOASTERMENU
////////////////////////////////////////
// Function: rollercoasterMenu - Main menu function for roller coaster
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void rollercoasterMenu()
{
  byte button                 = BUTTON_NONE;
  byte maxChars               = 9;  // Max number of characters in edit fields
  const byte yAdj             = 0;

  // Draw constant words
  printMenuTitle(_tx_10_rolllercoasterMenu, maxChars, yAdj);
  printMenuName(_tx_10_trigType,         0, yAdj);
  printMenuName(_tx_10_trigVal,          1, yAdj);
  printMenuName(_tx_10_resetTime,        2, yAdj);
  printMenuName(_tx_10_preFocus,         3, yAdj);
  printMenuName(_tx_10_numShots,         4, yAdj);
  printMenuName(_tx_10_timeBetweenShots, 5, yAdj);
  
  helpPrintCurSensorValue3(98, 1, 0);
  
  // Draw the changable values in this menu (only one can be selected at a time)
  button |= printMenuSetString(maxChars, 0, g_mPos == 0, EEPROM_ROLLERCOASTER_TRIGGER_TYPE, 3, 2, _tx_10_lowHighList, yAdj);
  button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_ROLLERCOASTER_TRIGGER_VAL, 3, 0, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 2, g_mPos == 2, EEPROM_ROLLERCOASTER_RESET_TIME, 2, 0, 9, yAdj);
  button |= printMenuSetString(maxChars, 3, g_mPos == 3, EEPROM_ROLLERCOASTER_PREFOCUS, 9, 2, _tx_10_prefocusList, yAdj);
  button |= printMenuSetNumber(maxChars, 4, g_mPos == 4, EEPROM_NUMBER_SHOTS, 1, 0, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 5, g_mPos == 5, EEPROM_TIMEBETWEEN_SHOTS, 3, 0, 9, yAdj);

  menuProcessButton(button);
  menuScroll(button, 6);
}

////////////////////////////////////////
// Function: projectileFunc - Handles roller coaster triggering
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void rollercoasterFunc()
{
  byte triggerType = eepromReadInt(EEPROM_ROLLERCOASTER_TRIGGER_TYPE);
  int triggerVal = eepromReadInt(EEPROM_ROLLERCOASTER_TRIGGER_VAL);
  int resetTime = eepromReadInt(EEPROM_ROLLERCOASTER_RESET_TIME);
  byte prefocus = eepromReadInt(EEPROM_ROLLERCOASTER_PREFOCUS);
  byte numShots = eepromReadInt(EEPROM_NUMBER_SHOTS);
  int timeBetweenShots = eepromReadInt(EEPROM_TIMEBETWEEN_SHOTS);
  unsigned long lastSensor1TrigTime = 0;

  attachInterrupt(0, activeButtonISR, LOW);
  
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_10_rolllercoasterMenu, 0xFF, 0);
    printMenuName(_tx_10_pressActivateToExit, 5, 0);
  }while( g_dogm.next() );

  if (prefocus)
  {
    setDevicePins(0, HIGH, LOW); // Pre-focus
    setDevicePins(1, HIGH, LOW); // Pre-focus
  }

  while(g_menuMode == MENUMODE_PHOTO)
  {
    int sensorVal1 = readSensorAnalog(0);
    int sensorVal2 = readSensorAnalog(1);
    byte trigger2 = 0;
    
    if (triggerType == 0) // trigger on low
    {
      if (sensorVal2 <= triggerVal)
      {
        trigger2 = 1;
      }
      if (sensorVal1 <= triggerVal)
      {
        lastSensor1TrigTime = millis();
      }
    }
    else
    {
      if (sensorVal2 >= triggerVal)
      {
        trigger2 = 1;
      }
      if (sensorVal1 >= triggerVal)
      {
        lastSensor1TrigTime = millis();
      }
    }
    
    if (trigger2)
    {
      byte i;
      unsigned long timeDiff = millis() - lastSensor1TrigTime;
      if ((timeDiff > 1) && (timeDiff < resetTime*1000))
      {
        lastSensor1TrigTime = 0;
        
        for (i=0; i<numShots; ++i)
        {
          setDevicePins(0, HIGH, HIGH);
          setDevicePins(1, HIGH, HIGH);
          
          delay(timeBetweenShots/2);
          if (prefocus)
          {
            setDevicePins(0, HIGH, LOW); // Pre-focus
            setDevicePins(1, HIGH, LOW); // Pre-focus
          }
          else
          {
            setDevicePins(0, LOW, LOW);
            setDevicePins(1, LOW, LOW);
          }
          delay(timeBetweenShots/2);
          if (prefocus)
          {
            setDevicePins(0, HIGH, LOW); // Pre-focus
            setDevicePins(1, HIGH, LOW); // Pre-focus
          }

          if (g_menuMode != MENUMODE_PHOTO)
          {
            break;
          }
        }
      }
    }
  }

  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);
  buttonDebounce(10); // 10ms debounce
}

////////////////////////////////////////
// Function: helpPrintCurSensorValue3 - This reads an analog pin and prints out the value to the LCD
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   row          - Row in Y direction 0 is top row
//   sensor       - The analog sensor you want to read from
// Returns:
//   None
////////////////////////////////////////
void helpPrintCurSensorValue3(byte x, byte row, byte sensor)
{
  static int sensorVal = readSensorAnalog(sensor);
  byte y = 46 - row * (g_dogm.charHeight+1);
  static unsigned long time = 0;
  
  if (millis() - time > 1000)
  {
    time = millis();
    sensorVal = readSensorAnalog(sensor);
  }
  
  g_dogm.setXY(x, y);
  g_dogm.print("/");
  g_dogm.print(sensorVal);
}
#endif

