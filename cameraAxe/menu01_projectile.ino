////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This mode uses a special sensor that has two gate triggers that go high when triggered.  The gates are
// seperated by SENSOR_DISTANCE units.  This mode uses digital signals instead of analog to improve
// speed (analog reads on ATMEL 168 take 100 micro seconds).  The time taken to trigger these sensors lets
// me calculate the velocity of the projectile.  Then assuming constant velocity (ie this doesn't work for
// objects being acclerated by gravity) it can caculate how long to wait until impact with the target (user
// provided distance in inches or cm).

#ifdef USE_PROJECTILEMENU
////////////////////////////////////////
// Function: projectileMenu - Main menu function for projectile sensor
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void projectileMenu()
{
  byte button                 = BUTTON_NONE;
  byte maxChars               = 4;  // Max number of characters in edit fields
  const byte yAdj             = 0;
  
  // Draw constant words
  printMenuTitle(_tx_01_projectileMenu, maxChars, yAdj);
  printMenuName(_tx_01_distance,        0, yAdj);
  printMenuName(_tx_01_lowHighTrigger,  1, yAdj);
  printMenuName(_tx_01_distanceUnits,   2, yAdj);
  
  // Draw the changable values in this menu (only one can be selected at a time)
  button |= printMenuSetNumber(maxChars, 0, g_mPos == 0, EEPROM_PROJECTILE_SENSOR_DISTANCE, 2, 1, 9, yAdj);
  button |= printMenuSetString(maxChars, 1, g_mPos == 1, EEPROM_PROJECTILE_SENSOR_LOW_HIGH, 4, 2, _tx_01_lowHighList, yAdj);
  button |= printMenuSetString(maxChars, 2, g_mPos == 2, EEPROM_PROJECTILE_INCH_CM,         4, 2, _tx_01_inchCmList, yAdj);
  menuProcessButton(button);
  menuScroll(button, 3);
}

////////////////////////////////////////
// Function: projectileFunc - Handles projectile triggering
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void projectileFunc()
{
  int targetDistance = eepromReadInt(EEPROM_PROJECTILE_SENSOR_DISTANCE);
  byte lowHigh       = eepromReadInt(EEPROM_PROJECTILE_SENSOR_LOW_HIGH);
  byte inchCm        = eepromReadInt(EEPROM_PROJECTILE_INCH_CM);
  unsigned long int distanceBetweenSensors;
  
  attachInterrupt(0, activeButtonISR, LOW);
  
  helpProjectileLCD(inchCm, 0, false);
  
  lowHigh = (lowHigh == 0) ? LOW : HIGH;
  
  if (inchCm == 0)
    distanceBetweenSensors = 200;  // Distance between sensors is 2.00 inches
  else
    distanceBetweenSensors = 508;  // Distance between sensors is 2.00 inches (or 5.08 cm)

  startNanoSec();
  
  while(g_menuMode == MENUMODE_PHOTO)
  {
    unsigned long startTime;
    unsigned long endTime;
    unsigned long impactTime;

    if (readSensorDigitalFast(0) == lowHigh)  // If sensor1 detects projectile
      {
      startTime =  nanoSec();

      // Look for sensor2 to detect projectile
      do {
        endTime = nanoSec();
        if (endTime - startTime > 1000000000)  // If we have waited 1 second and there has not been a detection projectile must have missed second sensor
        {
          helpProjectileLCD(inchCm, 0, true);
          endNanoSec();
          delay(5000);
          startNanoSec();
          helpProjectileLCD(inchCm, 0, false);
          endTime = 0;
          break;
        }
      } while(readSensorDigitalFast(1) != lowHigh);
      
      if (endTime)
      {
        unsigned long int elapsedTime;
        unsigned long int hundredthInchCmPerSec;

        resetTimer0();
        
        elapsedTime             = (endTime - startTime);// ? (endTime - startTime) : 1;
        hundredthInchCmPerSec   = (distanceBetweenSensors*1000000)/(elapsedTime/1000);
        impactTime              = (1000000000/hundredthInchCmPerSec)*10*targetDistance;  // If changing be careful about overflowing int32
        
        while (nanoSec() < impactTime)  // Wait for impact
        {}
                
        //trigger flash
        setDevicePins(0, HIGH, HIGH);
        setDevicePins(1, HIGH, HIGH);
        
        // Display how fast the projectile was moving
        helpProjectileLCD(inchCm, hundredthInchCmPerSec/100, false);
        endNanoSec();
        delay(FLASH_DURATION_MS);
        startNanoSec();

        // Turn off flash
        setDevicePins(0, LOW, LOW);
        setDevicePins(1, LOW, LOW);
        delay(500);
      }
    }
  }

  endNanoSec();
  buttonDebounce(10); // 10ms debounce
}


////////////////////////////////////////
// Function: helpProjectileLCD - Prints lcd menu to LCD
//
// Parameters:
//   inchCm        - 0=inches and 1=centimeters
//   inchCmPerSec  - The number of inches or centimeters traveled per second
//   sensorFailure - false= no failure, true=failure
// Returns:
//   None
////////////////////////////////////////
void helpProjectileLCD(byte inchCm, unsigned long int inchCmPerSec, boolean sensorFailure)
{
  boolean biggerUnits = false;
  
  if (!inchCm && inchCmPerSec >= 48)
  {
    inchCmPerSec /= 12;
    biggerUnits = true;
  }
  else if (inchCm && inchCmPerSec >= 400)
  {
    inchCmPerSec /= 100;
    biggerUnits = true;
  }
  
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_01_projectilePhotoMode, 0xFF, 0);
    
    g_dogm.setXY(0, 36);
    if (sensorFailure)
      g_dogm.print(getStr(_tx_01_sensor2DidNotTrigger));
    else
      g_dogm.print(getStr(_tx_01_readyForProjectile));

    if (inchCmPerSec)
    {
      g_dogm.setXY(0, 20);
  
      if (!inchCm)
      {
        if (biggerUnits)
          g_dogm.print(getStr(_tx_01_feet));
        else
          g_dogm.print(getStr(_tx_01_inches));
      }
      else
      {
        if (biggerUnits)
          g_dogm.print(getStr(_tx_01_meters));
        else
          g_dogm.print(getStr(_tx_01_centimeters));
      }
      g_dogm.print(getStr(_tx_01_sec));
      g_dogm.print(inchCmPerSec);
    }
    
    g_dogm.setXY(10, 0);
    g_dogm.print(getStr(_tx_01_pressActivateToExit));
  } 
  while( g_dogm.next() );
}
#endif

