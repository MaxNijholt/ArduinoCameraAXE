////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This mode connects using ic2 to a maxsonar sensor.

#ifdef USE_MAXSONARMENU

#include "Wire.h"
#define MAXSONAR_ADDR   0x70  //The Sensor ranging command has a value of 0x51
#define RANGE_CMD       0x51

////////////////////////////////////////
// Function: maxsonarMenu - Main menu function for maxsonar
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void maxsonarMenu()
{
  byte button                 = BUTTON_NONE;
  byte maxChars               = 4;  // Max number of characters in edit fields
  const byte yAdj             = 0;
  static boolean  firstRun    = true;
  
  if (firstRun)
  {
    Wire.begin();//Initiate Wire library for I2C communications with I2CXL‑MaxSonar‑EZ
    firstRun = false;
  }
  
  setSensorPinModes(0, INPUT, INPUT);
  setSensorPinModes(1, INPUT_PULLUP, INPUT_PULLUP);
  setSensorPinModes(1, OUTPUT, OUTPUT);
  setSensorPowerPin(0, HIGH);
  setSensorPowerPin(1, LOW);

  // Draw constant words
  if (g_mPos <= 5)
  {
    printMenuTitle(_tx_12_maxsonarMenu, maxChars, yAdj);
    printMenuName(_tx_12_trigWidth,         0, yAdj);
    printMenuName(_tx_12_cam1Delay,         1, yAdj);
    printMenuName(_tx_12_cam1LowLat,        2, yAdj);
    printMenuName(_tx_12_cam1Duration,      3, yAdj);
    printMenuName(_tx_12_cam2Delay,         4, yAdj);
    printMenuName(_tx_12_cam2LowLat,        5, yAdj);
    // Draw the changable values in this menu (only one can be selected at a time)
    button |= printMenuSetNumber(maxChars, 0, g_mPos == 0, EEPROM_MAXSONAR_TRIGGER_VAL, 3, 0, 7, yAdj);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_MAXSONAR_CAM1_DELAY, 3, 0, 9, yAdj);
    button |= printMenuSetString(maxChars, 2, g_mPos == 2, EEPROM_MAXSONAR_CAM1_LOW_LAT, 3, 2, _tx_12_noYesList, yAdj);
    button |= printMenuSetNumber(maxChars, 3, g_mPos == 3, EEPROM_MAXSONAR_CAM1_DURATION, 4, 0, 9, yAdj);
    button |= printMenuSetNumber(maxChars, 4, g_mPos == 4, EEPROM_MAXSONAR_CAM2_DELAY, 3, 0, 9, yAdj);
    button |= printMenuSetString(maxChars, 5, g_mPos == 5, EEPROM_MAXSONAR_CAM2_LOW_LAT, 3, 2, _tx_12_noYesList, yAdj);

    //Print distance value...
    helpPrintMaxsonar(90, 0);
  }
  else
  {
    printMenuTitle(_tx_12_maxsonarMenu, maxChars, yAdj);
    printMenuName(_tx_12_cam2Duration,      0, yAdj);
    printMenuName(_tx_12_timeBetweenShots,  1, yAdj);
  
    button |= printMenuSetNumber(maxChars, 0, g_mPos == 6, EEPROM_MAXSONAR_CAM2_DURATION, 4, 0, 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 7, EEPROM_MAXSONAR_TIME_BETWEEN_SHOTS, 4, 0, 9, yAdj);

  }

  menuProcessButton(button);
  menuScroll(button, 8);
}

////////////////////////////////////////
// Function: maxsonarFunc - Handles maxsonar triggering
//
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void maxsonarFunc()
{
  int triggerVal  = eepromReadInt(EEPROM_MAXSONAR_TRIGGER_VAL);
  int camDelay[2] = {eepromReadInt(EEPROM_MAXSONAR_CAM1_DELAY), eepromReadInt(EEPROM_MAXSONAR_CAM2_DELAY)};
  byte camLowLat[2]  = {eepromReadInt(EEPROM_MAXSONAR_CAM1_LOW_LAT), eepromReadInt(EEPROM_MAXSONAR_CAM2_LOW_LAT)};
  int camDuration[2] = {eepromReadInt(EEPROM_MAXSONAR_CAM1_DURATION), eepromReadInt(EEPROM_MAXSONAR_CAM2_DURATION)};
  int timeBetweenShots = eepromReadInt(EEPROM_MAXSONAR_TIME_BETWEEN_SHOTS);
  byte camPrefocus[2];
  byte i;

  attachInterrupt(0, activeButtonISR, LOW);
  
  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_12_maxsonarMenu, 0xFF, 0);
    printMenuName(_tx_12_pressActivateToExit, 5, 0);
  }while( g_dogm.next() );
  
  camPrefocus[0] = (camLowLat[0]) ? HIGH : LOW;
  camPrefocus[1] = (camLowLat[1]) ? HIGH : LOW;

  setSensorPinModes(0, INPUT, INPUT);
  setSensorPinModes(1, INPUT_PULLUP, INPUT_PULLUP);
  setSensorPinModes(1, OUTPUT, OUTPUT);
  setSensorPowerPin(0, HIGH);
  setSensorPowerPin(1, LOW);
  setDevicePins(0, camPrefocus[0], LOW);
  setDevicePins(1, camPrefocus[1], LOW);

  while(g_menuMode == MENUMODE_PHOTO)
  {
    int distance = maxsonarRangeReading();
    
    if (distance <= triggerVal)
    {
      unsigned long curTime = millis();
      unsigned long camStartTime[2] = {(curTime + camDelay[0]), (curTime + camDelay[1])};
      unsigned long camEndTime[2]   = {(camStartTime[0] + camDuration[0]), (camStartTime[1] + camDuration[1])};
      boolean go = true;

      while(go)
      {
        curTime = millis();

        for(i=0; i<2; ++i)
        {
          if (camStartTime[i] > curTime)
          {
            setDevicePins(i, camPrefocus[i], LOW);
          }
          else if (camEndTime[i] > curTime)
          {
            setDevicePins(i, HIGH, HIGH);
          }
          else
          {
            setDevicePins(i, camPrefocus[i], LOW);
          }
        }
        
        if ( (curTime > (camEndTime[0]+timeBetweenShots)) &&
             (curTime > (camEndTime[1]+timeBetweenShots)) )
        {
          go = false;
        }
      }
      delay(50); // Minimum 50ms delay (can be increased with time between shots menu option)
    }
  }

  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);
  buttonDebounce(10); // 10ms debounce
}

////////////////////////////////////////
// Function: maxsonarRangeReeding - Takes a distance measurement
// Parameters:
//   None
// Returns:
//   value read from sensor
////////////////////////////////////////
word maxsonarRangeReading()
{
  word range;
  byte i = 0;
  
  Wire.beginTransmission(MAXSONAR_ADDR);
  Wire.write(RANGE_CMD);
  Wire.endTransmission();
  Wire.requestFrom(MAXSONAR_ADDR, 2);
  while((Wire.available() < 2) && (i < 60))
  {
    ++i;
    delay(1); // Delay until there are values or until 60 ms has expired
    Wire.requestFrom(MAXSONAR_ADDR, 2);
  }

  Wire.requestFrom(MAXSONAR_ADDR, 2);
  
  if(Wire.available() >= 2)
  {
    byte HighByte = Wire.read();
    byte LowByte = Wire.read();
    range = word(HighByte, LowByte);  // range in cm
  }
  else
  { 
    range = 999;  // This should never happen so it's an error
  }
  
  return range;
}    

////////////////////////////////////////
// Function: helpPrintCurSensorValue4 - Prints out the value to the LCD
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   row          - Row in Y direction 0 is top row
// Returns:
//   None
////////////////////////////////////////
void helpPrintMaxsonar(byte x, byte row)
{
  int val;
  byte y = 46 - row * (g_dogm.charHeight+1);
  
  val = maxsonarRangeReading();
  
  g_dogm.setXY(x, y);
  g_dogm.print(val);
}

#endif

