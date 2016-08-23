////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
//  Updated 2016-03 by DML1333 - Fixed Valve Menu problem when Valve 2,3 or 4 had timing longer than Valve 1 -- Flash delay is now either 
//      based on the start of the whole sequence (Flash Based on Start=Yes) or is relative to the last drop from Valve 1 (=No)
//      and updated comments below to better explain drop sequencing; Auto Calibrate now displays the actual flash delay (from Start)
//       and Drop 2 Delay value while the sequence is proceeding.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This sensor times a solenoid valve opening to create one to three drops from one to four valves and fire a flash to take a picture.
// 
// Device 1 is optionally attached to your camera (alternately you can trigger the camera manually with a long exposure)
// Device 2 is connected to the flash
// Middle on Sensor1 is valve 1
// Middle on Sensor2 is valve 2
// Tip on Sensor1 is valve 3
// Tip on Sensor2 is valve 4
//
// The delays for each drop are timed from the END of the prior drop (incremental delays)
// The timing of drops for valves 1-4 are each computed from the start of Activation, not valve 1 all drops, then valve 2, ...
// For each Valve, here is the normal sequence
//   Wait for Start Offset
//   Open Valve x for Drop 1 Size (ms)
//   Wait for Drop 2 Delay (ms)
//   Open Valve x for Drop 2 Size (ms)
//   Wait for Drop 3 Delay (ms)
//   Open Valve x for Drop 3 Size (ms)
//   Wait for the remaining delay from Activation and then trigger device 2 (flash)
//   If Flash Delay is from Start (yes), then Flash Delay must be longer than the longest valve's drop sequence
//
//   Auto Calibrate - Starts by resetting the number of valves to 1 and the initial flash delay to 300 ms
//      It uses your Drop 1 Size setting, and INCREMENTS the flash delay by 5 ms until you press select (you've picked the tallest column)
//      It then begins drop 2 with a delay of the flash delay determined above minus 200 ms and the same size
//      Drop 2 Delay is then Decremented by 5 ms until you press select to indicate that the collision looks the best
//      Remember to reset the Flash Delay Auto Inc and Number of Shots to zero when not using the Auto Calibrate or else it will 
//      increment your chosen flash delay (probably not what you want).

#ifdef USE_VALVEMENU


////////////////////////////////////////
// Function: valveMenu - Handles menuing for the valve sensor
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void valveMenu()
{
  unsigned int numValves = eepromReadInt(EEPROM_VALVE_NUM_VALVES);
  byte button            = BUTTON_NONE;
  byte maxChars          = 3;  // Max number of characters in edit fields
  byte yAdj              = 0;
  
  // Override SetDefaultPins()
  setSensorPinModes(0, OUTPUT, OUTPUT);
  setSensorPinModes(1, OUTPUT, OUTPUT);
  setSensorPowerPin(0, LOW);
  setSensorPowerPin(1, LOW);
  setSensorPins(0, LOW, LOW);
  setSensorPins(1, LOW, LOW);

  if (g_mPos < 3)
  {
    button |= printMenuSetString(maxChars, 2, g_mPos == 2,  EEPROM_VALVE_FLASH_DELAY_BASE, 3, 2, _tx_02_noYesList, yAdj);
    printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
    printMenuName(_tx_02_calibrate,      0,  yAdj); 
    printMenuName(_tx_02_numValves,      1,  yAdj);
    printMenuName(_tx_02_flashDelayBase, 2,  yAdj);
    
    button |= calibrateSubMenu(g_mPos == 0);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 1,  EEPROM_VALVE_NUM_VALVES,         1, 0 , 4, yAdj);
    if (eepromReadInt(EEPROM_VALVE_NUM_VALVES) == 0)
    {
       eepromWriteInt(EEPROM_VALVE_NUM_VALVES, 1); // Hack - should be handled printMenuSetNumber...
    }
  }
  else if (g_mPos < 9)
  {  
    printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
    printMenuName2("V1", _tx_02_Drop1Delay,  0,  yAdj);
    printMenuName2("V1", _tx_02_Drop1Size,   1,  yAdj);
    printMenuName2("V1", _tx_02_Drop2Delay,  2,  yAdj);
    printMenuName2("V1", _tx_02_Drop2Size,   3,  yAdj);
    printMenuName2("V1", _tx_02_Drop3Delay,  4,  yAdj);
    printMenuName2("V1", _tx_02_Drop3Size,   5,  yAdj);
    button |= printMenuSetNumber(maxChars, 0,  g_mPos == 3,  EEPROM_VALVE_V1_DROP1_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1,  g_mPos == 4,  EEPROM_VALVE_V1_DROP1_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 2,  g_mPos == 5,  EEPROM_VALVE_V1_DROP2_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 3,  g_mPos == 6,  EEPROM_VALVE_V1_DROP2_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 4,  g_mPos == 7,  EEPROM_VALVE_V1_DROP3_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 5,  g_mPos == 8,  EEPROM_VALVE_V1_DROP3_SIZE,      3, 0 , 9, yAdj);
  }
  else if (g_mPos < 15)
  {
    printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
    printMenuName(_tx_02_flashDelay,        0, yAdj);
    printMenuName(_tx_02_flashDelayAutoInc, 1, yAdj);
    printMenuName(_tx_02_numShots,          2, yAdj);
    printMenuName(_tx_02_secBetwnShots,     3, yAdj);
    printMenuName(_tx_02_repeatShots,       4, yAdj);
    printMenuName(_tx_02_flash1,            5, yAdj);
    button |= printMenuSetNumber(maxChars, 0, g_mPos == 9,  EEPROM_VALVE_FLASH_DELAY,        3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 10,  EEPROM_VALVE_FLASH_INCREMENT,    2, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 2, g_mPos == 11,  EEPROM_VALVE_NUM_SHOTS,          3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 3, g_mPos == 12,  EEPROM_VALVE_SEC_BETWEEN_SHOTS,  2, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 4, g_mPos == 13, EEPROM_VALVE_REPEAT_SHOTS,       1, 0 , 9, yAdj);
    button |= printMenuSetString(maxChars, 5, g_mPos == 14, EEPROM_VALVE_FLASH1_PORT,        3, 2, _tx_02_camFlashList, yAdj); 
  }
  else if (g_mPos < 21)
  {
    printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
    printMenuName2("V2", _tx_02_Drop1Delay, 0, yAdj);
    printMenuName2("V2", _tx_02_Drop1Size,  1, yAdj);
    printMenuName2("V2", _tx_02_Drop2Delay, 2, yAdj);
    printMenuName2("V2", _tx_02_Drop2Size,  3, yAdj);
    printMenuName2("V2", _tx_02_Drop3Delay, 4, yAdj);
    printMenuName2("V2", _tx_02_Drop3Size,  5, yAdj);
    button |= printMenuSetNumber(maxChars, 0,  g_mPos == 15, EEPROM_VALVE_V2_DROP1_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1,  g_mPos == 16, EEPROM_VALVE_V2_DROP1_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 2,  g_mPos == 17, EEPROM_VALVE_V2_DROP2_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 3,  g_mPos == 18, EEPROM_VALVE_V2_DROP2_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 4,  g_mPos == 19, EEPROM_VALVE_V2_DROP3_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 5,  g_mPos == 20, EEPROM_VALVE_V2_DROP3_SIZE,      3, 0 , 9, yAdj);
  }
  else if (g_mPos < 27)
  {
    printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
    printMenuName2("V3", _tx_02_Drop1Delay, 0, yAdj);
    printMenuName2("V3", _tx_02_Drop1Size,  1, yAdj);
    printMenuName2("V3", _tx_02_Drop2Delay, 2, yAdj);
    printMenuName2("V3", _tx_02_Drop2Size,  3, yAdj);
    printMenuName2("V3", _tx_02_Drop3Delay, 4, yAdj);
    printMenuName2("V3", _tx_02_Drop3Size,  5, yAdj);
    button |= printMenuSetNumber(maxChars, 0, g_mPos == 21, EEPROM_VALVE_V3_DROP1_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 22, EEPROM_VALVE_V3_DROP1_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 2, g_mPos == 23, EEPROM_VALVE_V3_DROP2_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 3, g_mPos == 24, EEPROM_VALVE_V3_DROP2_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 4, g_mPos == 25, EEPROM_VALVE_V3_DROP3_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 5, g_mPos == 26, EEPROM_VALVE_V3_DROP3_SIZE,      3, 0 , 9, yAdj);
  }
  else
  {
    printMenuTitle(_tx_02_valveMenu, maxChars, yAdj);
    printMenuName2("V4", _tx_02_Drop1Delay, 0, yAdj);
    printMenuName2("V4", _tx_02_Drop1Size,  1, yAdj);
    printMenuName2("V4", _tx_02_Drop2Delay, 2, yAdj);
    printMenuName2("V4", _tx_02_Drop2Size,  3, yAdj);
    printMenuName2("V4", _tx_02_Drop3Delay, 4, yAdj);
    printMenuName2("V4", _tx_02_Drop3Size,  5, yAdj);
    button |= printMenuSetNumber(maxChars, 0, g_mPos == 27, EEPROM_VALVE_V4_DROP1_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 28, EEPROM_VALVE_V4_DROP1_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 2, g_mPos == 29, EEPROM_VALVE_V4_DROP2_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 3, g_mPos == 30, EEPROM_VALVE_V4_DROP2_SIZE,      3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 4, g_mPos == 31, EEPROM_VALVE_V4_DROP3_DELAY,     3, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 5, g_mPos == 32, EEPROM_VALVE_V4_DROP3_SIZE,      3, 0 , 9, yAdj);
  }

  menuProcessButton(button);
  menuScroll(button, numValves*6+9);
}

////////////////////////////////////////
// Function: valveFunc - Handles triggering for valve menu
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
// Middle on Sensor1 is valve 1
// Middle on Sensor2 is valve 2
// Tip on Sensor1 is valve 3
// Tip on Sensor2 is valve 4

void valveFunc()
{
  int numValves      = eepromReadInt(EEPROM_VALVE_NUM_VALVES);
  int flashDelayBase = eepromReadInt(EEPROM_VALVE_FLASH_DELAY_BASE);
  int flashDelay     = eepromReadInt(EEPROM_VALVE_FLASH_DELAY);
  int flashInc       = eepromReadInt(EEPROM_VALVE_FLASH_INCREMENT);
  int secBetwnShots  = eepromReadInt(EEPROM_VALVE_SEC_BETWEEN_SHOTS);
  int numShots       = eepromReadInt(EEPROM_VALVE_NUM_SHOTS);
  int repeatShots    = eepromReadInt(EEPROM_VALVE_REPEAT_SHOTS) + 1;
  int flash1         = eepromReadInt(EEPROM_VALVE_FLASH1_PORT);  // 0=camera  1=flash
  int vDrop1Delay[4] = {0,0,0,0};
  int vDrop1Size[4]  = {0,0,0,0};
  int vDrop2Delay[4] = {0,0,0,0};
  int vDrop2Size[4]  = {0,0,0,0};
  int vDrop3Delay[4] = {0,0,0,0};
  int vDrop3Size[4]  = {0,0,0,0};
  
  extern void runDrops( int numValves, int flashDelayBase, int flashDelay, int flashInc, int secBetwnShots, int numShots, int repeatShots, int flash1,
               int* vDrop1Delay, int *vDrop1Size, int* vDrop2Delay, int *vDrop2Size, int* vDrop3Delay, int *vDrop3Size);  // added by DML1333 for compatibility with IDE 1.6.6+
  
  vDrop1Delay[0]  = eepromReadInt(EEPROM_VALVE_V1_DROP1_DELAY);
  vDrop1Size[0]   = eepromReadInt(EEPROM_VALVE_V1_DROP1_SIZE);
  vDrop2Delay[0]  = eepromReadInt(EEPROM_VALVE_V1_DROP2_DELAY);
  vDrop2Size[0]   = eepromReadInt(EEPROM_VALVE_V1_DROP2_SIZE);
  vDrop3Delay[0]  = eepromReadInt(EEPROM_VALVE_V1_DROP3_DELAY);
  vDrop3Size[0]   = eepromReadInt(EEPROM_VALVE_V1_DROP3_SIZE);
  if(numValves > 1)
  {
    vDrop1Delay[1]  = eepromReadInt(EEPROM_VALVE_V2_DROP1_DELAY);
    vDrop1Size[1]   = eepromReadInt(EEPROM_VALVE_V2_DROP1_SIZE);
    vDrop2Delay[1]  = eepromReadInt(EEPROM_VALVE_V2_DROP2_DELAY);
    vDrop2Size[1]   = eepromReadInt(EEPROM_VALVE_V2_DROP2_SIZE);
    vDrop3Delay[1]  = eepromReadInt(EEPROM_VALVE_V2_DROP3_DELAY);
    vDrop3Size[1]   = eepromReadInt(EEPROM_VALVE_V2_DROP3_SIZE);
  }
  if(numValves > 2)
  {
    vDrop1Delay[2]  = eepromReadInt(EEPROM_VALVE_V3_DROP1_DELAY);
    vDrop1Size[2]   = eepromReadInt(EEPROM_VALVE_V3_DROP1_SIZE);
    vDrop2Delay[2]  = eepromReadInt(EEPROM_VALVE_V3_DROP2_DELAY);
    vDrop2Size[2]   = eepromReadInt(EEPROM_VALVE_V3_DROP2_SIZE);
    vDrop3Delay[2]  = eepromReadInt(EEPROM_VALVE_V3_DROP3_DELAY);
    vDrop3Size[2]   = eepromReadInt(EEPROM_VALVE_V3_DROP3_SIZE);
  }
  if (numValves > 3)
  {
    vDrop1Delay[3]  = eepromReadInt(EEPROM_VALVE_V4_DROP1_DELAY);
    vDrop1Size[3]   = eepromReadInt(EEPROM_VALVE_V4_DROP1_SIZE);
    vDrop2Delay[3]  = eepromReadInt(EEPROM_VALVE_V4_DROP2_DELAY);
    vDrop2Size[3]   = eepromReadInt(EEPROM_VALVE_V4_DROP2_SIZE);
    vDrop3Delay[3]  = eepromReadInt(EEPROM_VALVE_V4_DROP3_DELAY);
    vDrop3Size[3]   = eepromReadInt(EEPROM_VALVE_V4_DROP3_SIZE);
  }

  attachInterrupt(0, activeButtonISR, LOW);

  g_dogm.start();
  do 
  {
    printMenuTitle(_tx_02_valvePhotoMode, 0xFF, 0);
    printMenuName(_tx_02_valveActive, 5, 0);
  } 
  while( g_dogm.next() );

  runDrops( numValves, flashDelayBase, flashDelay, flashInc, secBetwnShots, numShots, repeatShots, flash1,
            vDrop1Delay, vDrop1Size, vDrop2Delay, vDrop2Size, vDrop3Delay, vDrop3Size);
  
  g_menuMode = MENUMODE_MENU;  // Always return to menu in this mode
  detachInterrupt(0);
}

////////////////////////////////////////
// Function: runDrops - Creates one or more drops to match request from input parameters
// Parameters:
//   Lots...
// Returns:
//   None
////////////////////////////////////////

void runDrops( int numValves, int flashDelayBase, int flashDelay, int flashInc, int secBetwnShots, int numShots, int repeatShots, int flash1,
               int* vDrop1Delay, int *vDrop1Size, int* vDrop2Delay, int *vDrop2Size, int* vDrop3Delay, int *vDrop3Size)
{
  unsigned int drop1on[4], drop1off[4], drop2on[4], drop2off[4], drop3on[4], drop3off[4]; // drop on/off  times for the 4 valves
  unsigned int maxtime, elapsedMilli;
  unsigned long start;
  int repeat;
  int origFlashDelay = flashDelay;
  int remainingFlashDelay = 0;  // Added by DML1333 2016-03
  byte i;

  if (!flashDelayBase)
  {
    // If flashDelayBase=No flashDelay was entered as incrememtal to valve 1 steps
    flashDelay = flashDelay + vDrop1Delay[0] + vDrop1Size[0] + vDrop2Delay[0] + vDrop2Size[0] + vDrop3Delay[0] + vDrop3Size[0];
//    flashDelay = max(0, flashDelay); DML1333 2016-03 no longer needed
  }

  numShots = (numShots == 0) ? 1 : numShots;

  do
  {
    --numShots;
    
    for(repeat = 0; repeat < repeatShots; ++repeat)
    {
      setSensorPowerPin(0, LOW);
      setSensorPowerPin(1, LOW);
      setSensorPinModes(0, OUTPUT, OUTPUT);
      setSensorPinModes(1, OUTPUT, OUTPUT);
      setSensorPins(0, LOW, LOW);
      setSensorPins(1, LOW, LOW);

      if(flash1==0)
      {
        setDevicePins(0, HIGH, HIGH);  // Trigger camera
        delay(400);// Give camera 400 ms to open shutter
      }

      for (i = 0; i < 4; ++i)
      {
        drop1on[i]  = vDrop1Delay[i];
        drop1off[i] = drop1on[i]  + vDrop1Size[i];
        drop2on[i]  = drop1off[i] + vDrop2Delay[i];
        drop2off[i] = drop2on[i]  + vDrop2Size[i];
        drop3on[i]  = drop2off[i] + vDrop3Delay[i];
        drop3off[i] = drop3on[i]  + vDrop3Size[i];
      }

      maxtime      = max(max(drop3off[0], drop3off[1]),  max(drop3off[2], drop3off[3]));
      remainingFlashDelay = flashDelay - maxtime;  // added by DML1333 2016-03
      elapsedMilli = 0;
      start        = millis();
      
      while (start == millis())  // This makes sure we are starting at the beginning of a millisecond for consistency purposes
      {}
      start        = millis();
     
      setSensorPins(0, HIGH, LOW);  // Turn on V1
     
      //Loop for triggering all valves events
      while (elapsedMilli <= maxtime)
      {
        byte valveLowHigh[4] = {LOW, LOW, LOW, LOW};
        elapsedMilli = millis() - start;

        for( i=0; i< 4; ++i)
        {
          if (     elapsedMilli >= drop3off[i])
            valveLowHigh[i] = LOW;
          else if (elapsedMilli >= drop3on[i])
            valveLowHigh[i] = HIGH;
          else if (elapsedMilli >= drop2off[i])
            valveLowHigh[i] = LOW;
          else if (elapsedMilli >= drop2on[i])
            valveLowHigh[i] = HIGH;
          else if (elapsedMilli >= drop1off[i])
            valveLowHigh[i] = LOW;
          else if (elapsedMilli >= drop1on[i])
            valveLowHigh[i] = HIGH;
        }
        
        setSensorPins(0, valveLowHigh[0], valveLowHigh[2]);
        setSensorPins(1, valveLowHigh[1], valveLowHigh[3]);
      }

      //Wait for Flash Delay and then Trigger flash
      delay(remainingFlashDelay);  // changed by DML1333 2016-03
    
      // Trigger flash
      setDevicePins(0, HIGH, HIGH);  // If flash1 is in camera mode it doesn't hurt to reset and if in flash mode needs to be set
      setDevicePins(1, HIGH, HIGH);
      delay(FLASH_DURATION_MS);
      setDevicePins(1, LOW, LOW);  // Turn Flash off
      setDevicePins(0, LOW, LOW);  // Turn Camera off
  
      for (i = 0; i < secBetwnShots; ++i)
      {
        if ( g_menuMode == MENUMODE_MENU )
          break;
          
        delay(1000);
      }
    }

    if (eepromReadInt(EEPROM_VALVE_FLASH_INCREMENT))
    {
      origFlashDelay+=flashInc;
      eepromWriteInt(EEPROM_VALVE_FLASH_DELAY, origFlashDelay);
      eepromClamp(EEPROM_VALVE_FLASH_DELAY, 0, 999);
      flashDelay = eepromReadInt(EEPROM_VALVE_FLASH_DELAY);
    }

  } while ( (numShots > 0) && (g_menuMode != MENUMODE_MENU) );
}

////////////////////////////////////////
// Function: testDrop - Helper function to create a single drop during calibration mode
// Parameters:
//   Lots...
// Returns:
//   None
////////////////////////////////////////
void testDrop(int flashDelay, int drop1Size, int drop2Delay, int drop2Size)
{
  int numValves      = 1;
  int flashDelayBase = 1;
  int flashInc       = 0;
  int secBetwnShots  = 0;
  int numShots       = 0;
  int repeatShots    = 1;
  int flash1         = 0;
  int vDrop1Delay[4] = {0,0,0,0};
  int vDrop1Size[4]  = {0,0,0,0};
  int vDrop2Delay[4] = {0,0,0,0};
  int vDrop2Size[4]  = {0,0,0,0};
  int vDrop3Delay[4] = {0,0,0,0};
  int vDrop3Size[4]  = {0,0,0,0};
  
  vDrop1Delay[0]  = 0;
  vDrop1Size[0]   = drop1Size;
  vDrop2Delay[0]  = drop2Delay;
  vDrop2Size[0]   = drop2Size;
  vDrop3Delay[0]  = 0;
  vDrop3Size[0]   = 0;
  
  g_menuMode = MENUMODE_PHOTO;
  runDrops( numValves, flashDelayBase, flashDelay, flashInc, secBetwnShots, numShots, repeatShots, flash1,
            vDrop1Delay, vDrop1Size, vDrop2Delay, vDrop2Size, vDrop3Delay, vDrop3Size);
  g_menuMode = MENUMODE_MENU;
}

////////////////////////////////////////
// Function: calibrateSubMenu - A submenu used to calibrate good values to collide drops
// Parameters:
//   Lots...
// Returns:
//   button press value
////////////////////////////////////////
byte calibrateSubMenu(byte selected)
{
  byte button = BUTTON_NONE;

  if (selected)
  {
    g_dogm.setBox(113, 46, 128, 53); // Display box so people know it's highlighted

    button = detectButtonPress(true);
    if (button == BUTTON_SELECT)
    {
      int numValves      = 1;   //eepromReadInt(EEPROM_VALVE_NUM_VALVES);
      int flashDelayBase = 1;   //eepromReadInt(EEPROM_VALVE_FLASH_DELAY_BASE);  // 0 = based off V1 last drop; 1 = based off start of sequence
      int flashDelay     = 300; //eepromReadInt(EEPROM_VALVE_FLASH_DELAY);
      int flashInc       = 0;   //eepromReadInt(EEPROM_VALVE_FLASH_INCREMENT);
      int secBetwnShots  = 0;   //eepromReadInt(EEPROM_VALVE_SEC_BETWEEN_SHOTS);
      int numShots       = 0;   //eepromReadInt(EEPROM_VALVE_NUM_SHOTS);
      int repeatShots    = 0;   //eepromReadInt(EEPROM_VALVE_REPEAT_SHOTS) + 1;
      int flash1         = 0;   //eepromReadInt(EEPROM_VALVE_FLASH1_PORT);  // 0=camera  1=flash
    
      int drop1Delay     = 0;  //eepromReadInt(EEPROM_VALVE_V1_DROP1_DELAY);
      int drop1Size      = eepromReadInt(EEPROM_VALVE_V1_DROP1_SIZE);
      int drop2Delay     = 0;  //eepromReadInt(EEPROM_VALVE_V1_DROP2_DELAY);
      int drop2Size      = 0;  //eepromReadInt(EEPROM_VALVE_V1_DROP2_SIZE);
      int drop3Delay     = 0;  //eepromReadInt(EEPROM_VALVE_V1_DROP3_DELAY);
      int drop3Size      = 0;  //eepromReadInt(EEPROM_VALVE_V1_DROP3_SIZE);
      int i, j;
      byte exitloop      = 0;
      
      // Display text for the submenu
      g_dogm.start();
      do 
      {
        printMenuTitle(_tx_02_calibrationMenu, 0xFF, 0);
        printMenuName(_tx_02_calibrationText1, 0, 0);
        printMenuName(_tx_02_calibrationText2, 1, 0);
        printMenuName(_tx_02_calibrationText3, 2, 0);
        printMenuName(_tx_02_calibrationText4, 3, 0);
        printMenuName(_tx_02_calibrationText5, 5, 0);
      } 
      while( g_dogm.next() );
      
      button = BUTTON_NONE;      
      // Wait for select button press to start action
      while (button != BUTTON_SELECT)
      {
        button = detectButtonPress(true);
      }
      
      // Increment flashDelay until max splash height
      for (i=0; i<400; i+=5)
      {
        // Display text for the submenu
        g_dogm.start();
        do 
        {
          byte row = 4;
          printMenuTitle(_tx_02_calibrationMenu, 0xFF, 0);
          printMenuName(_tx_02_calibrationText1, 0, 0);
          printMenuName(_tx_02_calibrationText2, 1, 0);
          printMenuName(_tx_02_calibrationText3, 2, 0);
          printMenuName(_tx_02_calibrationText4, 3, 0);
          byte y = 46 - row * (g_dogm.charHeight+1);
          g_dogm.setXY(0, y);
          g_dogm.print(flashDelay + i);  // Changed by DML1333 20116-03 to display the actual flash delay rather than offset
          printMenuName(_tx_02_calibrationText5, 5, 0);
        } while( g_dogm.next() );

        for (j=0; j<5000; ++j) // Delay 5 seconds
        {
          delay(1);
          button = detectButtonPress(true);
          if (button == BUTTON_SELECT)
          {
            exitloop = 1;
            break;
          }
          else if (button == BUTTON_UP)
          {
            i-=5;
          }
          else if (button == BUTTON_DOWN)
          {
            i+=5;
          }
        }
        if (exitloop)
        {
          exitloop = 0;
          break;
        }
        testDrop(flashDelay+i, drop1Size, drop2Delay, drop2Size);
      }
      flashDelay = flashDelay + i;
      
      // Decrement drop2Delay until drops collide
      drop2Delay = flashDelay-200;
      drop2Size  = drop1Size;
      for (i=0; i<drop2Delay; i+=5)
      {
        // Display text for the submenu
        g_dogm.start();
        do 
        {
          byte row = 4;
          printMenuTitle(_tx_02_calibrationMenu, 0xFF, 0);
          printMenuName(_tx_02_calibrationText1, 0, 0);
          printMenuName(_tx_02_calibrationText2, 1, 0);
          printMenuName(_tx_02_calibrationText3, 2, 0);
          printMenuName(_tx_02_calibrationText4, 3, 0);
          byte y = 46 - row * (g_dogm.charHeight+1);
          g_dogm.setXY(0, y);
          g_dogm.print(drop2Delay-i);  // Changed by DML1333 20116-03 to display the actual delay value
          printMenuName(_tx_02_calibrationText5, 5, 0);
        } while( g_dogm.next() );

        for (j=0; j<5000; ++j) // Delay 5 seconds
        {
          delay(1);
          button = detectButtonPress(true);
          if (button == BUTTON_SELECT)
          {
            exitloop = 1;
            break;
          }
          else if (button == BUTTON_UP)
          {
            i-=5;
          }
          else if (button == BUTTON_DOWN)
          {
            i+=5;
          }
        }
        if (exitloop)
        {
          exitloop = 0;
          break;
        }

        testDrop(flashDelay, drop1Size, drop2Delay-i, drop2Size);
      }
      drop2Delay = drop2Delay - i;
      button = BUTTON_NONE;

      // Save the new values for the calibrated valve sensor
      eepromWriteInt(EEPROM_VALVE_NUM_VALVES, numValves);
      eepromWriteInt(EEPROM_VALVE_FLASH_DELAY_BASE, flashDelayBase);
      eepromWriteInt(EEPROM_VALVE_FLASH_DELAY, flashDelay);
      eepromWriteInt(EEPROM_VALVE_FLASH_INCREMENT, flashInc);
      eepromWriteInt(EEPROM_VALVE_SEC_BETWEEN_SHOTS, secBetwnShots);
      eepromWriteInt(EEPROM_VALVE_NUM_SHOTS, numShots);
      eepromWriteInt(EEPROM_VALVE_REPEAT_SHOTS, repeatShots);
      eepromWriteInt(EEPROM_VALVE_FLASH1_PORT, flash1);
  
      eepromWriteInt(EEPROM_VALVE_V1_DROP1_DELAY, drop1Delay);
      eepromWriteInt(EEPROM_VALVE_V1_DROP1_SIZE, drop1Size);
      eepromWriteInt(EEPROM_VALVE_V1_DROP2_DELAY, drop2Delay);
      eepromWriteInt(EEPROM_VALVE_V1_DROP2_SIZE, drop2Size);
      eepromWriteInt(EEPROM_VALVE_V1_DROP3_DELAY, drop3Delay);
      eepromWriteInt(EEPROM_VALVE_V1_DROP3_SIZE, drop3Size);
    }
  }
 
  return button;
}

#endif

