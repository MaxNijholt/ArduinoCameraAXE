////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This mode uses a light sensor plugged into sensor1 and gives the user over bulb duratations to make it
// easier to capture fireworks.

#ifdef USE_FIREWORKSMENU
////////////////////////////////////////
// Function: fireworksMenu - Main menu function for fireworks
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void fireworksMenu()
{
  byte button                 = BUTTON_NONE;
  byte maxChars               = 7;  // Max number of characters in edit fields
  const byte yAdj             = 0;

  // Draw constant words
  printMenuTitle(_tx_08_fireworksMenu, maxChars, yAdj);
  printMenuName(_tx_08_bulb,    0, yAdj);
  printMenuName(_tx_08_trigVal, 1, yAdj);
  printMenuName(_tx_08_numTrig, 2, yAdj);
  
  helpPrintCurSensorValue2(108, 1, 0);
  
  // Draw the changable values in this menu (only one can be selected at a time)
  button |= printMenuSetNumber(maxChars, 0, g_mPos == 0, EEPROM_FIREWORKS_BULB,    2, 1, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_FIREWORKS_TRIGVAL, 3, 0, 9, yAdj);
  button |= printMenuSetNumber(maxChars, 2, g_mPos == 2, EEPROM_FIREWORKS_NUMTRIG, 2, 0, 9, yAdj);
  menuProcessButton(button);
  menuScroll(button, 3);
}

////////////////////////////////////////
// Function: projectileFunc - Handles fireworks triggering
//   The algorithm waits for a burst of light.  Then it triggers the camera.  Then it waits for the bulb time
//    or the number of bursts of light (maxNumTrig) to be reached.  As soon as one of these conditions are hit
//    the camera triggering is stopped.
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void fireworksFunc()
{
  int bulb        = eepromReadInt(EEPROM_FIREWORKS_BULB);
  int trigVal     = eepromReadInt(EEPROM_FIREWORKS_TRIGVAL);
  byte maxNumTrig = max(eepromReadInt(EEPROM_FIREWORKS_NUMTRIG), 1);
  unsigned long resetTime = 0xffffffff; // Edge case issue if time wraps around but that takes about 50 days of continious running
  byte prevSensorTrig = 0; // Tracks if the sensor was triggered or not on prvious pass
  byte numTrig = 0;

  attachInterrupt(0, activeButtonISR, LOW);
  
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_08_fireworksMode, 0xFF, 0);
    printMenuName(_tx_08_pressActivateToExit, 5, 0);
  }while( g_dogm.next() );
  
  setDevicePins(0, HIGH, LOW); // Pre-focus
  setDevicePins(1, HIGH, LOW); // Pre-focus
  
  while(g_menuMode == MENUMODE_PHOTO)
  {
    int sensorVal = readSensorAnalog(0);
    
    if ((sensorVal > trigVal) && (prevSensorTrig == 0))
    {
      prevSensorTrig = 1;
      if (numTrig == 0);
      {
        resetTime = millis() + bulb*100;
        setDevicePins(0, HIGH, HIGH); // Trigger Camera1
        setDevicePins(1, HIGH, HIGH); // Trigger Camera2
        delay(200); // Give camera time register signals
      }
      ++numTrig;
    }
    else if ((sensorVal <= trigVal) && (prevSensorTrig == 1))
    {
      prevSensorTrig = 0;
    }
    
    if ((numTrig >= maxNumTrig) || (millis() >= resetTime))
    {
      setDevicePins(0, HIGH, LOW); // Stop triggering Camera1
      setDevicePins(1, HIGH, LOW); // Stop triggering Camera2
      delay(200); // Give camera time register signals
      prevSensorTrig = 0;
      numTrig = 0;
      resetTime = 0xffffffff;
    }
    
    delay(1); // No need to run loop super fast since that might lead to debounce issues with sensor reads
  }

  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);
  buttonDebounce(10); // 10ms debounce
}

////////////////////////////////////////
// Function: helpPrintCurSensorValue2 - This reads an analog pin and prints out the value to the LCD
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   row          - Row in Y direction 0 is top row
//   sensor       - The analog sensor you want to read from
// Returns:
//   None
////////////////////////////////////////
void helpPrintCurSensorValue2(byte x, byte row, byte sensor)
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

