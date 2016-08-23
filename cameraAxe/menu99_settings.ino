////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010, 2011, 2012, 2016)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
// Function: generalSettingsMenu - Handles menuing for general settings
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void generalSettingsMenu()
{
  byte button                     = BUTTON_NONE;
  byte maxChars                   = 8;
  const byte yAdj                 = 0;

  printMenuTitle(_tx_99_generalSettings, maxChars, 0);
  g_dogm.setXY(128-(strlen(getStr((_tx_99_version))))*g_dogm.charWidth, 56);
  g_dogm.print(getStr(_tx_99_version));
  
  printMenuName(_tx_99_backlight,      0, yAdj);
  printMenuName(_tx_99_startupMode,    1, yAdj); 
  printMenuName(_tx_99_presetGroup,    2, yAdj);
  printMenuName(_tx_99_delayDecPlaces, 3, yAdj);
  
#ifdef USE_IRREMOTEMENU
  printMenuName(_tx_99_camera1Type,    4, yAdj);
  printMenuName(_tx_99_camera2Type,    5, yAdj);
#endif

  // Draw the changeable values in this menu (only one can be selected at a time)
  button |= printMenuSetString(maxChars, 0, g_mPos == 0, EEPROM_GENERAL_LCD_BACKLIGHT, 6, 3, _tx_99_backlightList, yAdj);
  button |= printMenuSetString(maxChars, 1, g_mPos == 1, EEPROM_GENERAL_STARTUP_MODE,  5, 2, _tx_99_startupList, yAdj);
  button |= printMenuSetString(maxChars, 2, g_mPos == 2, EEPROM_GENERAL_PRESET_GROUP,  8, 4, _tx_99_presetList, yAdj);
  button |= printMenuSetString(maxChars, 3, g_mPos == 3, EEPROM_GENERAL_DELAY_DECIMALS,1, 2, _tx_99_delayDecPlaceList, yAdj);
  
#ifdef USE_IRREMOTEMENU
  button |= printMenuSetString(maxChars, 4, g_mPos == 4, EEPROM_DEVICE1_TYPE,  8, 8, _tx_99_deviceTypeList, yAdj);
  button |= printMenuSetString(maxChars, 5, g_mPos == 5, EEPROM_DEVICE2_TYPE,  8, 8, _tx_99_deviceTypeList, yAdj);
  //if these have changed, also change the global variable
  setShutterFunc(0,eepromReadInt(EEPROM_DEVICE1_TYPE));
  setShutterFunc(1,eepromReadInt(EEPROM_DEVICE2_TYPE));
#endif
    
  switch (eepromReadInt(EEPROM_GENERAL_LCD_BACKLIGHT))
  {
    case 0:
      lcdPower(LOW);
      break;
    case 1:
      lcdPower(HIGH);
      break;
  }

    switch (eepromReadInt(EEPROM_GENERAL_DELAY_DECIMALS))
  {
    case 0:
      g_Delay_Scaler = 100;
	  break;
    case 1:
      g_Delay_Scaler = 1;
      break;
  }

  
  menuProcessButton(button);

#ifdef USE_IRREMOTEMENU
  menuScroll(button, 6);
#else
  menuScroll(button, 4);
#endif
}

////////////////////////////////////////
// Function: generalSettingsFunc - Handles Photo Mode for the General Settings menu
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////

void generalSettingsFunc()
{
    // No photo mode for the settings menu
    g_menuMode = MENUMODE_MENU;
}