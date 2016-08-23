////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This mode connects to the a mobile phone/tablet and is triggered by an app on that device.  Currently this is compatible with the 
// Triggertrap mobile app.  I recommend supporting that project by purchasing their Triggertrap mobile dongle.  See www.triggertrap.com
// for more information.

#ifdef USE_DONGLEMENU
////////////////////////////////////////
// Function: dongleMenu - Main menu function for dongle
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void dongleMenu()
{
  byte button                 = BUTTON_NONE;
  byte maxChars               = 0xFE;  // Max number of characters in edit fields
  const byte yAdj             = 0;

  // Draw constant words
  printMenuTitle(_tx_11_dongleMenu, maxChars, yAdj);
  printMenuName(_tx_11_triggertrap, 5, yAdj);
  
  // Draw the changable values in this menu (only one can be selected at a time)
  
  button = detectButtonPress(true);
  menuProcessButton(button);
  menuScroll(button, 0);
}

////////////////////////////////////////
// Function: dongleFunc - Handles dongle triggering
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void dongleFunc()
{
  int triggerVal = 10;

  attachInterrupt(0, activeButtonISR, LOW);
  
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_11_dongleMenu, 0xFF, 0);
    printMenuName(_tx_11_pressActivateToExit, 5, 0);
  }while( g_dogm.next() );
  
  setSensorPinModes(0, INPUT, INPUT);
  setSensorPinModes(1, INPUT, INPUT);
  setSensorPowerPin(0, LOW);
  setSensorPowerPin(1, LOW);

  while(g_menuMode == MENUMODE_PHOTO)
  {
    int sensorValMid;
    int sensorValTip;
    byte focus = LOW;
    byte shutter = LOW;
    byte i;

    // Average look for any high samples since it is a sound wave
    for(i=0; i<100; ++i)
    {
      sensorValMid = readSensorAnalog(0);
      sensorValTip = readSensorAnalogTip(0);
      focus   |= (sensorValMid < triggerVal) ? LOW : HIGH;
      shutter |= (sensorValTip < triggerVal) ? LOW : HIGH;
    }

    setDevicePins(0, focus, shutter);
    setDevicePins(1, focus, shutter);
  }

  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);
  buttonDebounce(10); // 10ms debounce
}

#endif

