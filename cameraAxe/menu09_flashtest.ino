////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ricky Marek
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-Non-Commercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This menu can be used to check the Flash response time.
// Device 1 is used to fire the Flash (That is, this is the place you need to connect the Flash device)
// Sensor 1 is to be used by the Light Sensor.
// Note: this test can be also used to check the Camera response time together with its flash.
//
//
// The work flow is:
// 1) Connect the Flash To CA.
// 2) Connect the light sensor to Sensor1 CA.
// 3) Configure the two values on the menu:
//    - Flash Recycle time: Time in seconds that the flash may need to recharge (from 1 to 99), defaults to 10
//    - Loop Count: Times to repeat the test (0 to 99), defaults to 0 (Zero means no loop, just one iteration).
// 4) Press the "Activate" button and Camera Axe will do:
//    1) Store the current time as startTime
//    2) Trigger the Flash
//    3) Wait for sensing an signal in the Light Sensor. (If no light recorded, it will timeout after 1sec)
//    4) Once the Light Sensor triggered, record the time as riseTime.
//    5) Wait for sensing that the signal in the Light Sensor went back to low.
//    6) Once the light Sensor went back to low, record the time as fallTime
//    7) Compute and display the Flash Delay and Flash Duration from the previously recorded values. 
//    8) Wait for the Flash Recycle time.
//    9) If Loop Count is not zero, repeat the whole procedure for the number of times stored in Loop Count
//   10) Exit this menu.

#ifdef USE_FLASH_TEST_MENU
////////////////////////////////////////
// Function: flashTestMenu - Handles menuing for the Flash Benchmark Menu
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void flashTestMenu()
{
    byte button     = BUTTON_NONE;
    byte maxChars   = 8;  // Max number of characters in edit fields
    const byte yAdj = 0;

    printMenuTitle(_tx_09_flashTestMenu, maxChars, yAdj);
    printMenuName(_tx_09_flashRecycleTime,      0, yAdj);
    printMenuName(_tx_09_flashLoopCount,        1, yAdj);

    // Draw the changeable values in this menu (only one can be selected at a time)
    button |= printMenuSetNumber(maxChars, 0, g_mPos == 0, EEPROM_FLASH_RECYCLE_TIME,    2, 0 , 9, yAdj);
    button |= printMenuSetNumber(maxChars, 1, g_mPos == 1, EEPROM_FLASH_LOOP_COUNT,      2, 0 , 9, yAdj);

    menuProcessButton(button);
    menuScroll(button, 2);
}

////////////////////////////////////////
// Function: helpDrawFlashProgress - Draws Flash Benchmark Results Menu to LCD
// Parameters:
//   flashLag       - Flash Lag time in microseconds.
//   flashDuration  - Flash Duration time in microseconds
//   recycleTime   - Time left to wait till the Flash will recycle (Countdown counter)
//   loopCount     - Current iteration count.
//   isValid       - Flag used to determine the validity of the three time-tick values.
// Returns:
//   None
////////////////////////////////////////
void helpDrawFlashProgress(unsigned long flashLag, 
                           unsigned long flashDuration, 
                           int recycleTime, 
                           int loopCount, 
                           bool isValid)
{
    const byte yAdj                = 0;  

    g_dogm.start();
    do 
    {
        printMenuTitle(_tx_09_flashTestPhotoMode, 0xFF, yAdj);
        printMenuName(_tx_09_flashRecycleTime,       0, yAdj);
        printMenuName(_tx_09_flashLoopCount,         1, yAdj);
        printMenuName(_tx_09_flashLagTime,           2, yAdj);
        printMenuName(_tx_09_flashDurationTime,      3, yAdj);

        // Draw constant lines
        g_dogm.setVLine(85, 18, 54);
        g_dogm.setBox(0, 54, 127, 55);
        g_dogm.setBox(0, 36, 127, 36);
        g_dogm.setBox(0, 18, 127, 18);

        lcdPrintZeros(93, 45, decimalToBcd(recycleTime), 2, 0);
        lcdPrintZeros(93, 37, decimalToBcd(loopCount), 2, 0);

        if (isValid)
        {
            g_dogm.setXY(93, 28);
            g_dogm.print(flashLag);
            g_dogm.setXY(93, 19);
            g_dogm.print(flashDuration);
        }


        g_dogm.setXY(10, 0);
        g_dogm.print(getStr(_tx_09_pressActivateToExit));
    }
    while( g_dogm.next() );
}

////////////////////////////////////////
// Function: helpDrawFlashResults - Draws Flash Benchmark Results Menu to LCD
// Parameters:
//   valid Count    - Number of valid entries collected. (used to compute the average)
//   lagMax         - Max Lag value recorded.
//   lagMin         - Min Lag value recorded.
//   lagMean        - Mean LAg value recorded.
//   durMax         - Max Duration value recorded.
//   durMin         - Min Duration value recorded.
//   durMean        - Mean Duration value recorded.
// Returns:
//   None
////////////////////////////////////////
void helpDrawFlashResults(unsigned long lagMean,
                          unsigned long lagMax, 
                          unsigned long lagMin, 
                          unsigned long durMean, 
                          unsigned long durMax, 
                          unsigned long durMin,  
                          int validCount)
{
    const byte yAdj   = 0;  
    const int lagCol  = 24;
    const int durCol  = 66;
    const int xOffset  = 3;
     
  g_dogm.start();
  do 
  {
      // 
      //  Flash Test Results
      // ------+--------------------------------
      //       | Lag(us)        | Duration(us)
      // ------+----------------+---------------
      // Mean  |                | 
      // Max   |                |
      // Min   |                |
      // -----------------------+---------------
      // Valid Items            |
      // ---------------------------------------
      //      Press Activate to Exit
      printMenuTitle(_tx_09_flashTestResultsMode, 0xFF, yAdj);
      printMenuName(_tx_09_dummy,                    0, yAdj);
      printMenuName(_tx_09_meanValue,                1, yAdj);
      printMenuName(_tx_09_maxValue,                 2, yAdj);
      printMenuName(_tx_09_minValue,                 3, yAdj);
      printMenuName(_tx_09_validItems,               4, yAdj);

      // Draw constant lines
      g_dogm.xorBox(0, 54, 127, 63);
      g_dogm.setVLine(lagCol, 18, 54);
      g_dogm.setVLine(durCol, 9, 54);
      g_dogm.setBox(0,  45, lagCol, 54);
      g_dogm.setBox(0,  54, 127, 55);
      g_dogm.setBox(0,  45, 127, 45);
      g_dogm.setBox(0,  18, 127, 18);
      g_dogm.setBox(0,  9, 127, 9);
      g_dogm.setXY(lagCol+xOffset, 46);
      g_dogm.print(getStr(_tx_09_flashLagTime));
      g_dogm.setXY(durCol+xOffset, 46);
      g_dogm.print(getStr(_tx_09_flashDurationTime));
      g_dogm.setXY(lagCol+xOffset, 37);
      g_dogm.print(lagMean);
      g_dogm.setXY(durCol+xOffset, 37);
      g_dogm.print(durMean);
      g_dogm.setXY(lagCol+xOffset, 28);
      g_dogm.print(lagMax);
      g_dogm.setXY(durCol+xOffset, 28);
      g_dogm.print(durMax);
      g_dogm.setXY(lagCol+xOffset, 19);
      g_dogm.print(lagMin);
      g_dogm.setXY(durCol+xOffset, 19);
      g_dogm.print(durMin);
      g_dogm.setXY(durCol+xOffset, 10);
      g_dogm.print(validCount);

      g_dogm.setXY(10, 0);
      g_dogm.print(getStr(_tx_09_pressActivateToExit));
  }
  while( g_dogm.next() );
}

////////////////////////////////////////
// Function: flashTestFunc - Handles triggering for Flash Benchmark Menu
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void flashTestFunc()
{
    unsigned long startTime;
    unsigned long timeoutTime;
    unsigned long riseTime;
    unsigned long fallTime=0;
    unsigned long flashLag;
    unsigned long flashDuration;
    unsigned long lagMax;
    unsigned long lagMin;
    unsigned long lagSum;
    unsigned long durMax;
    unsigned long durMin;
    unsigned long durSum;
    
    int           loopCount;
    int           validCount;
    int           recycleTime;
    int           recycleCount;
    boolean       isValid;       // True when data is valid, false when timeout occurred.

    loopCount           = eepromReadInt(EEPROM_FLASH_LOOP_COUNT);
    recycleTime         = eepromReadInt(EEPROM_FLASH_RECYCLE_TIME);
    lagMax       = 0;
    lagMin       = 0xFFFFFFFF;
    lagSum       = 0;
    durMax  = 0;
    durMin  = 0xFFFFFFFF;
    durSum  = 0;
    validCount   = 0;

    attachInterrupt(0, activeButtonISR, LOW);

    loopCount = (loopCount > 0) ? loopCount : 1;
    recycleTime = (recycleTime > 0) ? recycleTime : 1;

    do
    {
        --loopCount;
        isValid = false;

        resetTimer0();  // Just to avoid the overflow potential
        startNanoSec();

        //====== Start Real-Time-Section =======
        startTime   =  nanoSec();   // Store the startTime time-tick. (in nanosec)
        timeoutTime = startTime + 1000000000;

        // Trigger the Flash
        setDevicePins(0, HIGH, HIGH);
        setDevicePins(1, HIGH, HIGH);

        do
        {
            riseTime = nanoSec();
        }
        while((LOW == readSensorDigitalFast(0)) && (riseTime < timeoutTime));

        if (riseTime < timeoutTime)
        {
            do
            {
                fallTime = nanoSec();
            }
            while((HIGH == readSensorDigitalFast(0)) && (fallTime < timeoutTime));
            
            isValid = (fallTime < timeoutTime);
        } 

        //====== End Real-Time-Section =======
        endNanoSec();

        if (isValid)
        {
            delay(FLASH_DURATION_MS); // TODO: May not be correct!
        }
        
        // Turn off flash
        setDevicePins(0, LOW, LOW);
        setDevicePins(1, LOW, LOW);


        recycleCount = recycleTime;
        
        flashLag      = (riseTime - startTime)/1000;
        flashDuration = (fallTime - riseTime)/1000;
        
        while ((loopCount > 0) && (recycleCount > 0) && (g_menuMode != MENUMODE_MENU))
        {
            helpDrawFlashProgress(flashLag, flashDuration, recycleCount, loopCount, isValid);
            recycleCount--;
            delay(1000);
        }

        if (isValid)
        {
          validCount++;
          lagMax       = max(lagMax, flashLag);
          lagMin       = min(lagMin, flashLag);
          lagSum       += flashLag;
          durMax  = max(durMax, flashDuration);
          durMin  = min(durMin, flashDuration);
          durSum  += flashDuration;
        }
        
    } while ( (loopCount > 0) && (g_menuMode != MENUMODE_MENU) );

    if ((g_menuMode != MENUMODE_MENU) && (validCount != 0))
    {
        helpDrawFlashResults(lagSum/validCount, lagMax, lagMin, durSum/validCount, durMax, durMin,   validCount);
  
        while(g_menuMode != MENUMODE_MENU) { /* Do Nothing */ };
    }
    
    g_menuMode = MENUMODE_MENU;  // Always return to menu in this mode
    detachInterrupt(0);
    buttonDebounce(10); // 10 ms debounce
}
#endif

