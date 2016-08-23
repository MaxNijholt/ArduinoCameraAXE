////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
// Lightning Menu and Func Copyright D. M. Lenardon 2014, Open Source under same conditions
// Release Notes
// Version released with CameraAxe 5.6 beta 5
// Version released with CameraAxe 5.6 beta 6
// Version released with CameraAxe 5.6 beta 7
// Version released with CameraAxe 5.6 beta 8  --- Lightning Menu V2.0
//    -PROGMEM declarations in CameraAxe.ino Updated to standard so this version will compile under Arduino IDE 1.5+
//    -Added Advanced Display Mode to display the details of the last 4 strikes, default to Simple Display
//    -updated code and displays to handle sensor values up to 1023 rather than 999
//    -Added a Working max sensor value, currently at 1012 measured for Osram SFH310, (1016-1018 for Vishay TEPT4400)
//    -Lowered default Update Reference Period to .2 sec
//    -Complete restructuring of the code to improve speed, Prior version took ~55 us between sensor reads, 
//        after adding the details display this went up to 88 us, but with restructuring of the code, 
//        it is now down to ~27 us (the average time just to readAnalogSensor in a tight loop is ~22.5 us,
//        so 27 us is really good)
//    -added the Too Hi warning in Menu Mode, in addition to both Simple and Advanced Displays
//    -In Photo Mode, displays are updated only once every second because the display takes 50-70 milliseconds
//        and that would take too much time away from looking for strikes if too frequent
//    -Also the new structure of the code insures that the reference value does not get updated during a strike
//        and that a strike is limited to two (2) seconds to guard against lockup if ambient changes quickly
//    -Updated the .doc file in the docs directory to reflect all of the V2.0 changes
// Version released with CameraAxe 5.6 beta 10 DML  --- Lightning Menu V2.1
//	  - Modified the strike duration to be in microseconds instead of milliseconds
//    - Modified Bulb Sec to have 2 digits after the decimal (00.00 - 99.99 seconds)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef USE_LIGHTNINGMENU
////////////////////////////////////////
// Function: LightningMenu - Similar to AdvancedSensorMenu, but with a specific threshold algorithm for Lightning.  
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////

// *** Global Constants for Lightning Menu and Functions
const int workingMaxSensorVal = 1012; // 1012 max observed for OsramSFH310; 1016 for Vishay TEPT4400

void lightningMenu()
{
  byte button = BUTTON_NONE;
  int sensorVal = 0;
  byte maxChars = 17;  // Max number of characters in edit fields
  byte yAdj = 0;
  char c_warning[7] = " ";
 
  int eepromSensorTrigVal = eepromReadInt(EEPROM_LTG_SENSOR_TRIG_VAL);


  // clear camera/flash pins back to default (especially reset camera focus)
  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);

  
  printMenuTitle(_tx_13_lightningMenu, maxChars, yAdj);
  printMenuName(_tx_13_trigVal,         0, yAdj);
  printMenuName(_tx_13_UpdtMS,          1, yAdj);
  printMenuName(_tx_13_DisplayMode,	    2, yAdj);
  printMenuName(_tx_13_Camera1Camera2,  3, yAdj);
  printMenuName(_tx_13_prefocus,        4, yAdj);
  printMenuName(_tx_13_bulbSec,         5, yAdj);

  // Draw constant lines
  g_dogm.setVLine(41, 0, 53);
  g_dogm.setVLine(86, 0, 27);
  g_dogm.setBox(0, 27, 127, 27);
  g_dogm.setBox(41, 18, 127, 18);

  // Display current sensor value
  sensorVal = readSensorAnalog(0);
  g_dogm.setXY(60, 46);
  g_dogm.print("/");
  g_dogm.setXY(65, 46);
  g_dogm.print(sensorVal);
  if( (sensorVal + eepromSensorTrigVal) >= workingMaxSensorVal)
    strcpy( c_warning, "Too Hi");
  else
    strcpy( c_warning, " ");
  g_dogm.setXY(95, 46);
  g_dogm.print(c_warning);

  // Draw the changeable values in this menu (only one can be selected at a time)
  button |= printMenuSetNumber(maxChars,   0, g_mPos == 0, EEPROM_LTG_SENSOR_TRIG_VAL,        3, 0, 9, yAdj);
  button |= printMenuSetString(maxChars-9, 0, g_mPos == 1, EEPROM_LTG_SENSOR2_TRIG_NULL,      1, 1, _tx_13_nullnull, yAdj);
  button |= printMenuSetNumber(maxChars,   1, g_mPos == 2, EEPROM_LTG_SENSOR_UPD_BASE_PERIOD, 4, 0, 9,  yAdj);
  button |= printMenuSetString(maxChars-9, 1, g_mPos == 3, EEPROM_LTG_SENSOR2_TRIG_NULL,      1, 1, _tx_13_nullnull, yAdj);
  button |= printMenuSetString(maxChars,   2, g_mPos == 4, EEPROM_LTG_PHOTO_MENU_SIMPLE_ADV,  8, 2, _tx_13_DisplayModeList, yAdj);
  button |= printMenuSetString(maxChars-9, 2, g_mPos == 5, EEPROM_LTG_SENSOR2_TRIG_NULL,      1, 1, _tx_13_nullnull, yAdj);
  button |= printMenuSetString(maxChars,   4, g_mPos == 6, EEPROM_LTG_DEVICE_PREFOCUS1,       3, 2, _tx_13_noYesList, yAdj);
  button |= printMenuSetString(maxChars-9, 4, g_mPos == 7, EEPROM_LTG_DEVICE_PREFOCUS2,       3, 2, _tx_13_noYesList, yAdj);
  button |= printMenuSetNumber(maxChars,   5, g_mPos == 8, EEPROM_LTG_DEVICE_CYCLE1,          2, 2, 9, yAdj);
  button |= printMenuSetNumber(maxChars-9, 5, g_mPos == 9, EEPROM_LTG_DEVICE_CYCLE2,          2, 2, 9, yAdj);

  menuProcessButton(button);
  
  if (!g_editVal)
  {
    switch (button)
    {
    case BUTTON_LEFT:
    case BUTTON_RIGHT:
      {
        if (g_mPos%2) // is odd
          --g_mPos;
        else          // is even
        ++g_mPos;
      }
      break;
    case BUTTON_UP:
      {
        if (g_mPos<=1)
          g_mPos += 8;
        else
          g_mPos -= 2;
      }
      break;
    case BUTTON_DOWN:
      {
        if (g_mPos>=8)
          g_mPos -= 8;
        else
          g_mPos += 2;
      }
      break;
    }
  }
}

////////////////////////////////////////
// Function: lightningFunc - Handles camera/flash triggering for lightning similar to the advancedSensorFunc
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////

void lightningFunc()
{
  int sensorVal = -1;
  int referenceSensorVal = -1;
  int currentDif = 0;
  unsigned long ReferenceUpdateTimeMS = 0;
  int triggerCount = 0;
  boolean inStrikeCycle = false;        // logical indicating that we are in a strike cycle
  int ypos = 0;
  unsigned long deviceCycleTimesMS[2] = { 0, 0 };
  unsigned long curTimeMS;
  byte d;  // device number 0/1
  boolean TriggerTooHigh = false;
  unsigned long TimeToDisplayMS = 0;
  
// variables for the Advanced Photo Display - last 4 strike details  
  int strk_num[4] = { 0, 0, 0, 0 };
  int ref[5] = { 0, 0, 0, 0, 0 };  // extra index for temp storage of ref at beginning of strike
  int peak[4] = { 0, 0, 0, 0 };
  unsigned long strk_duration_us[4] = { 0, 0, 0, 0 };
  int peakOfStrike = 0;
  unsigned long StrikeStartTimeUs;  // used for strike duration display
  unsigned long StrikeStartTimeMS; // used for 2-second max strike duration to prevent lock-up with ambient or sensitivity changes
  
  int eepromSensorTrigVal = eepromReadInt(EEPROM_LTG_SENSOR_TRIG_VAL);
  // eepromDeviceCycles units are hundredths of a second
  int eepromDeviceCycles[2] = { 
    eepromReadInt(EEPROM_LTG_DEVICE_CYCLE1),       eepromReadInt(EEPROM_LTG_DEVICE_CYCLE2)     };
  int eepromDevicePrefocuses[2] = { 
    eepromReadInt(EEPROM_LTG_DEVICE_PREFOCUS1),    eepromReadInt(EEPROM_LTG_DEVICE_PREFOCUS2)     };
  int eepromPhotoMenuSimpleAdv = eepromReadInt(EEPROM_LTG_PHOTO_MENU_SIMPLE_ADV);
  int eepromSensorUpdateRefPeriod = eepromReadInt(EEPROM_LTG_SENSOR_UPD_BASE_PERIOD);  // units are milliseconds

  extern void  LTGdisplayDetailsOnLCD ( int eepromPhotoMenuSimpleAdv, int *referenceSensorVal, int *sensorVal, int *currentDif,
        boolean TriggerTooHigh, int *eepromSensorTrigVal, int *eepromSensorUpdateRefPeriod, int *triggerCount,
        int *peakOfStrike, int *strk_num, int *ref, int *peak, unsigned long *strk_duration_us); // added by DML1333 for compatibility with IDE 1.6.6+
        
  attachInterrupt(0, activeButtonISR, LOW);

  // Initialize Prefocus for both Cam/Flash ports
  for ( d = 0; d < 2; d++ )
  {
    if (eepromDevicePrefocuses[d])
      setDevicePins(d, HIGH, LOW);
    else
      setDevicePins(d, LOW, LOW);
  }
  triggerCount = 0;
  referenceSensorVal = readSensorAnalog(0);  // initialize reference base
  curTimeMS = millis();  // initialize the current time to start Reference and Display timers
  ReferenceUpdateTimeMS = curTimeMS + (unsigned long) eepromSensorUpdateRefPeriod ;  // initialize the update timer
  TimeToDisplayMS = curTimeMS + 1000; // Update the display only once per second because it takes ~50-70 ms to do
  
  while (g_menuMode == MENUMODE_PHOTO)
  {
    // Loop checking for a strike (curval - ref > trigger)
    while ( !inStrikeCycle && g_menuMode == MENUMODE_PHOTO)
    {
      curTimeMS = millis();  // capture the current time
      sensorVal = readSensorAnalog(0);
      currentDif = sensorVal - referenceSensorVal;
      if (currentDif > eepromSensorTrigVal)
      {
        // Begin a new strike -- strike lasts until sensor1 goes back below threshold or 2 seconds (bail out)
        inStrikeCycle = true;
        //Trigger both devices
        setDevicePins(0, HIGH, HIGH);
        setDevicePins(1, HIGH, HIGH);
        triggerCount = triggerCount + 1;
        peakOfStrike = sensorVal; // initialize the peak reading for this strike
        StrikeStartTimeUs = micros();  // initialize the strike start for duration measure
        StrikeStartTimeMS = curTimeMS;  // start clock for 2-second max check
        ref[4] = referenceSensorVal;  // save reference value at beginning of strike in index 4
        //Set up Cycle Times for Both Devices
        for ( d = 0; d < 2; d++ )
        {
          if( eepromDeviceCycles[d] != 0 )
            deviceCycleTimesMS[d] = curTimeMS + (unsigned long) eepromDeviceCycles[d]*10; //eeprom value is integer hundredths of sec. (10 ms)
          else
            deviceCycleTimesMS[d] = curTimeMS + g_minShutterHoldMs; // min hold time to activate shutter
            // known issue: If Prefocus is not set, some camera will sometimes not trigger unless the pins are held high for >=150MS
        }
        break;
      }
      
      // Not yet in a strike event
      // Is it time to update the Reference Value?
      if( curTimeMS >= ReferenceUpdateTimeMS )
      {
        referenceSensorVal = sensorVal;          // Update the threshold reference base value to current value 
        ReferenceUpdateTimeMS = curTimeMS + (unsigned long) eepromSensorUpdateRefPeriod;  // Update Timer 
        if( (referenceSensorVal + eepromSensorTrigVal) >= workingMaxSensorVal)
          // At top of Sensor range - trigVal too high - can't trigger
          TriggerTooHigh = true;  // Display a warning warning, but update ReferenceValue anyway, else continual trigger
        else
          TriggerTooHigh = false;
      }

      //Is it time to display values on the LCD?
      if( curTimeMS >= TimeToDisplayMS )
      {
        LTGdisplayDetailsOnLCD( eepromPhotoMenuSimpleAdv, &referenceSensorVal, &sensorVal, &currentDif, TriggerTooHigh,
        &eepromSensorTrigVal, &eepromSensorUpdateRefPeriod, &triggerCount, &peakOfStrike, strk_num, ref, peak, strk_duration_us );
        
        TimeToDisplayMS = curTimeMS + 1000; // Update the display only once per second because it takes ~50-70 ms to do

      }
    }  // End of loop looking for start of a strike
    
    // Begin loop looking for end of strike and handling DeviceCycles
    while ( inStrikeCycle && g_menuMode == MENUMODE_PHOTO)
    {
      sensorVal = readSensorAnalog(0);
      peakOfStrike = max( peakOfStrike, sensorVal);
      currentDif = sensorVal - referenceSensorVal;
      // Strike is done if current value is back below threshold or 2 seconds max to prevent lockups
      if (currentDif <= eepromSensorTrigVal)
      // Lightning may often have multiple short flashes/pulses, so this might just be the end of the first pulse
      {
        inStrikeCycle = false;
        break;
      }
      curTimeMS = millis();
      if (curTimeMS > (StrikeStartTimeMS + 2000))
      {
        // Sensor value has been above threshold for >2 seconds - must be an ambient or sensor sensitivity change
        // Just end the strike and reset the Reference level
        inStrikeCycle = false;
        referenceSensorVal = sensorVal;          // Update the threshold reference base value to current value 
        ReferenceUpdateTimeMS = curTimeMS + (unsigned long) eepromSensorUpdateRefPeriod; // reset the Ref Update clock
        break;
      }
        
      // check each device for completion of cycle time
      for ( d = 0; d < 2; d++ )
      {
        if( deviceCycleTimesMS[d] && (deviceCycleTimesMS[d] < millis()) )
        {
          deviceCycleTimesMS[d] = 0;
          if (eepromDevicePrefocuses[d])
            setDevicePins(d, HIGH, LOW);
          else
            setDevicePins(d, LOW, LOW);
        }
      }
    }
    
    // Strike cycle just finished
    // Move the prior 3 strike details down in the arrays and store the details for this strike that just completed
    for (int irow = 2; irow >= 0; --irow)
    {
      strk_num[irow + 1] = strk_num[irow];
      ref[irow + 1] = ref[irow];
      peak[irow + 1] = peak[irow];
      strk_duration_us[irow + 1] = strk_duration_us[irow];
    }
    strk_num[0] = triggerCount;
    ref[0] = ref[4];   // use temp copy of reference value set at beginning of strike because it could have changed
    peak[0] = peakOfStrike;
    strk_duration_us[0] = micros() - StrikeStartTimeUs + 14; // Add 14 microseconds as 1/2 the estimated time between AnalogReads in strike detection
    
    // Loop until DeviceCycles (Bulb TImer) is completed for both devices
    while ( deviceCycleTimesMS[0] || deviceCycleTimesMS[1] )
    {
      curTimeMS = millis();  // capture the current time
      sensorVal = readSensorAnalog(0);  // Keep the current value up to date and update peak & duration for secondary flashes
      currentDif = sensorVal - referenceSensorVal;
      // Check if light value has gone back up over threshold -- secondary pulses/flashes
      if (currentDif > eepromSensorTrigVal)
			{
				strk_duration_us[0] = micros() - StrikeStartTimeUs + 14; // Extend the strike duration to cover secondary flashes during deviceCycles
				peak[0] = max( peak[0] , sensorVal);  // Update the peak if needed
			}


      // check each device for completion of cycle time
      for ( d = 0; d < 2; d++ )
      {
        if( deviceCycleTimesMS[d] && (deviceCycleTimesMS[d] < curTimeMS) )
        {
          deviceCycleTimesMS[d] = 0;
          if (eepromDevicePrefocuses[d])
            setDevicePins(d, HIGH, LOW);
          else
            setDevicePins(d, LOW, LOW);
        }
      }
    }
    
    // Recheck if current value is below threshold and if so reset Reference
    sensorVal = readSensorAnalog(0);
    currentDif = sensorVal - referenceSensorVal;
    if (currentDif < eepromSensorTrigVal)
    {
      referenceSensorVal = sensorVal;          // Update the threshold reference base value to current value 
      ReferenceUpdateTimeMS = curTimeMS + (unsigned long) eepromSensorUpdateRefPeriod; // reset the Ref Update clock
    }
    // If still above threshold, keep old Reference to allow a retrigger to capture more secondary flashes / pulses
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
  //
  // Out of PHOTO mode now, so let the buttons settle down
  //
  buttonDebounce(200); // 200 ms debounce
  //
  // clear camera/flash back to default (especially reset camera prefocus)
  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);
  peakOfStrike = 0;  // zero out the peak before going back to MENU
  // Now back to MENU mode  
}

void  LTGdisplayDetailsOnLCD ( int eepromPhotoMenuSimpleAdv, int *referenceSensorVal, int *sensorVal, int *currentDif,
        boolean TriggerTooHigh, int *eepromSensorTrigVal, int *eepromSensorUpdateRefPeriod, int *triggerCount,
        int *peakOfStrike, int *strk_num, int *ref, int *peak, unsigned long *strk_duration_us)
{
  
  int ypos = 0;
  char TrigOrWarn[7] = "      ";
  char c_warning[7] = " ";
  String strTrig = "       ";
  
  g_dogm.start();
  // Display all of the interesting values while in PhotoMode
  do 
  {
    if( TriggerTooHigh )
      {
      strTrig = "Too Hi ";
      strcpy( c_warning, "Too Hi");
      }
    else
      {
        strTrig = "Trg";                  //Builds text
        strTrig += *eepromSensorTrigVal; //Concats TrigVal
        strcpy( c_warning, " ");
      }
      strTrig.toCharArray(TrigOrWarn, 7);

      if ( eepromPhotoMenuSimpleAdv == 0)  // Simple Display
    {
      printMenuTitle(_tx_13_lightningPhotoModeSimple, 0xFF, 0); 
      printMenuName(_tx_13_ReferenceBase, 0, 0);
      printMenuName(_tx_13_CurrentValDif, 1, 0);
      printMenuName(_tx_13_ThresholdTriggerValue, 2, 0);
      printMenuName(_tx_13_UpdateRefMS, 3, 0);
      printMenuName(_tx_13_TrigCntPeakVal, 4, 0);
      g_dogm.setXY(85, 46);
      g_dogm.print(*referenceSensorVal);            // Display the reference base value
      g_dogm.setXY(85, 37);
      g_dogm.print(*sensorVal);                     // Display the current value
      g_dogm.setXY(105, 37);
      g_dogm.print("/");
      g_dogm.setXY(110, 37);
      g_dogm.print(*currentDif);     // Display the difference + or - on the same line as current value
      g_dogm.setXY(75, 28);
      g_dogm.print(c_warning);
      g_dogm.setXY(105, 28);
      g_dogm.print("+");
      g_dogm.setXY(110, 28);
      g_dogm.print(*eepromSensorTrigVal);
      g_dogm.setXY(85, 19);
      g_dogm.print( *eepromSensorUpdateRefPeriod );
      g_dogm.setXY(85, 10);
      g_dogm.print(*triggerCount);
      g_dogm.print("/");
      g_dogm.print(*peakOfStrike);
      g_dogm.setXY(10, 0);
      printMenuName(_tx_13_pressActivateToExit, 5, 0);
    }
    else				// Advanced Display
    {
      printMenuTitle(_tx_13_lightningPhotoModeAdvanced, 0xFF, 0);
      g_dogm.setXY(0, 46);
      g_dogm.print("r");
      g_dogm.setXY(5, 46);
      g_dogm.print(*referenceSensorVal);
      g_dogm.setXY(30, 46);
      g_dogm.print("c/d");
      g_dogm.setXY(45, 46);
      g_dogm.print(*sensorVal);
      g_dogm.setXY(65, 46);
      g_dogm.print("/");
      g_dogm.setXY(70, 46);
      g_dogm.print(*currentDif);
      g_dogm.setXY(95, 46);
      g_dogm.print(TrigOrWarn);
          
      printMenuName(_tx_13_lightningPhotoModeAdvTitles, 1, 0);
      for (int strk_row = 0; strk_row <= 3; ++strk_row)
      {
        ypos = 28 - strk_row * 9;
        g_dogm.setXY(0, ypos);
        g_dogm.print( strk_num[strk_row] );
        g_dogm.setXY(25, ypos);
        g_dogm.print( ref[strk_row] );
        g_dogm.setXY(50, ypos);
        g_dogm.print( peak[strk_row] );
        g_dogm.setXY(80, ypos);
        g_dogm.print(strk_duration_us[strk_row]);
      }
    }
  } 
  while( g_dogm.next() );
  
  return;
  }

#endif
