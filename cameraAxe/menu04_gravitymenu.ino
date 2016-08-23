////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Andrew Morgan (Copyright 2010, 2011, 2012)
// Modified by Maurice Ribble
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This mode is based on the projectile menu but takes into account gravity.

#ifdef USE_GRAVITYMENU
////////////////////////////////////////
// Function: gravityMenu - Main menu function for the projectile sensor in gravity mode
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void gravityMenu()
{
  byte button                 = BUTTON_NONE;
  byte maxChars               = 4;  // Max number of characters in edit fields
  const byte yAdj             = 0;
  
  // Draw constant words
  printMenuTitle(_tx_04_gravityMenu, maxChars, yAdj);
  printMenuName(_tx_04_distance,      0, yAdj);
  printMenuName(_tx_04_distanceUnits, 1, yAdj);
  printMenuName(_tx_04_shutterLag,    2, yAdj);
  
  // Draw the changable values in this menu (only one can be selected at a time)
  button |= printMenuSetNumber(maxChars, 0, g_mPos == 0, EEPROM_GRAVITY_SENSOR_DISTANCE, 2, 1, 9, yAdj);
  button |= printMenuSetString(maxChars, 1, g_mPos == 1, EEPROM_GRAVITY_INCH_CM, 4, 2, _tx_04_inchCmList, yAdj);
  button |= printMenuSetNumber(maxChars, 2, g_mPos == 2, EEPROM_GRAVITY_SHUTTER_LAG,     3, 0, 9, yAdj);

  menuProcessButton(button);
  menuScroll(button, 3);
}

////////////////////////////////////////
// Function: gravityFunc - Handles gravity based triggering
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void gravityFunc()
{
  int targetDistance  = eepromReadInt(EEPROM_GRAVITY_SENSOR_DISTANCE);
  byte inchCm         = eepromReadInt(EEPROM_GRAVITY_INCH_CM);
  int shutterLag      = eepromReadInt(EEPROM_GRAVITY_SHUTTER_LAG);
  unsigned long int distanceBetweenSensors;
  unsigned long int gravity;
  
  attachInterrupt(0, activeButtonISR, LOW);
  
  helpGravityLCD(inchCm, 0, 0, 0, false);
    
  if (inchCm == 0)
  {
    distanceBetweenSensors = 200;  // Distance between sensors is 2.00 inches
    gravity = 38609; // in hundredths in /sec^2 32.174 ft/sec^2 * 12 in/ft * 100 hundredthsInch/in 
  }
  else
  {
    distanceBetweenSensors = 508;  // Distance between sensors is 2.00 inches (or 5.08 cm)
    gravity = 98000; // 9.8 m/sec^2 * 100 cm/m * 100 hundredthsCm/cm
  }
  
  resetTimer0();  // Just to avoid the overflow potential
  
  while(g_menuMode == MENUMODE_PHOTO)
  {
    unsigned long startTime;
    unsigned long endTime;

    if (readSensorDigitalFast(0) == LOW)  // If sensor1 detects projectile
    {
      startTime =  micros();

      if (shutterLag)
      {
        setDevicePins(0, HIGH, HIGH);
        delay(shutterLag);
      }

      // Look for sensor2 to detect projectile
      do {
        endTime = micros();
        if (endTime - startTime > 1000000)  // If we have waited 1 second and there has not been a detection.  Projetile must have missed second sensor
        {
          // Turn off camera
          // triggerDevicePins(0, LOW, LOW, LOW); // commented out by PTB
          setDevicePins(0, LOW, LOW);             // added by PTB
          helpGravityLCD(inchCm, 0, 0, 0, true);
          delay(5000);
          helpGravityLCD(inchCm, 0, 0, 0, false);
          endTime = 0;
          break;
        }
      } while(readSensorDigitalFast(1) != LOW);
      
      if (endTime)
      {
        unsigned long int elapsedTime;
        unsigned long int impactTime;
        unsigned long int totalDistance; // in hundredthsInchCm
        float t1; // time to get to first sensor
        float startingHeight;
        float totalTime;


        //  Do all the calculations to determine when the shot should be taken (impactTime)
        elapsedTime = (endTime - startTime);  //time to travel between the sensors in milliseconds        
        t1 = (((float)distanceBetweenSensors*1000000000000.0)/((float)gravity*(float)elapsedTime))-((float)elapsedTime/2.0); // time from starting point to first sensor
        startingHeight = ((float)gravity*t1*t1)/2000000000000.0;  // starting height above the first sensor
        totalDistance = (long)startingHeight + distanceBetweenSensors + targetDistance*10;  // total distance from starting height to the target spot for the photo
        totalTime = sqrt(2000000.0*(float)totalDistance/(float)gravity)*1000.0; // total time from starting height to the target spot for the photo 
        impactTime = startTime-(long)t1+(long)totalTime;  // impact time using the on board timer 0
        
        while (micros() < impactTime)  // Wait for impact
        {}
                
        // trigger flash
        setDevicePins(1, HIGH, HIGH);
        
        // Display some stats on the object's fall
        helpGravityLCD(inchCm, startingHeight, elapsedTime, totalTime-t1, false);
        delay(5000);

        // Turn off camera and flash
        setDevicePins(1, LOW, LOW);
        setDevicePins(0, LOW, LOW);
      }
      
      resetTimer0();  // Just to avoid the overflow potential
    }
  }

  buttonDebounce(10); // 10 ms debounce
}


////////////////////////////////////////
// Function: helpGravityLCD - Prints lcd menu to LCD
//
// Parameters:
//   inchCm        - 0=inches and 1=centimeters
//   inchCmPerSec  - The number of inches or centimeters traveled per second
//   sensorFailure - false= no failure, true=failure
// Returns:
//   None
////////////////////////////////////////
void helpGravityLCD(byte inchCm, unsigned long int startingInchCm, unsigned long int sensorTime, unsigned long int fallTime, boolean sensorFailure)
{
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_04_gravityPhotoMode, 0xFF, 0);
    
    g_dogm.setXY(0, 40);
    if (sensorFailure)
      g_dogm.print(getStr(_tx_04_sensor2DidNotTrigger));
    else
      g_dogm.print(getStr(_tx_04_readyForObject));

    if (startingInchCm > 0)
    {
      g_dogm.setXY(0, 30);
      g_dogm.print(getStr(_tx_04_startingHeight));
      float startingHeight = ((float)startingInchCm)/100.0;
      g_dogm.print(startingHeight);
  
      if (!inchCm)
      {
        g_dogm.print(getStr(_tx_04_inch));
      }
      else
      {
        g_dogm.print(getStr(_tx_04_cm));
      }
      if (sensorTime > 0 && fallTime > 0)
      {
        g_dogm.setXY(0, 22);
        g_dogm.print(getStr(_tx_04_sensorTime));
        g_dogm.print(sensorTime/1000);
        g_dogm.print(getStr(_tx_04_ms));
        g_dogm.setXY(0, 14);
        g_dogm.print(getStr(_tx_04_fallTime));
        g_dogm.print(fallTime/1000);
        g_dogm.print(getStr(_tx_04_ms));
      }
    }
    
    g_dogm.setXY(10, 0);
    g_dogm.print(getStr(_tx_04_pressActivateToExit));
  } 
  while( g_dogm.next() );
}
#endif
