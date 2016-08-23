////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_ADVANCEDMENU
////////////////////////////////////////
// Function: advancedSensorMenu - Handles menu for the generic sensor/flash menu.  
// Lots of flexibility to combine different uses of the two sensors and two devices.
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////

void advancedSensorMenu()
{
  byte button = BUTTON_NONE;
  static int staticVal1a[2]      = { 
    0xFFFF, 0xFFFF     }; // Need bit 16 to be 1 or helpPrintCurSensorValue doesn't work correctly
  static int staticVal1b[2]      = { 
    0xFFFF, 0xFFFF     }; // Need bit 16 to be 1 or helpPrintCurSensorValue doesn't work correctly
  static int staticVal2a[2]      = { 
    0xFFFF, 0xFFFF     }; // Need bit 16 to be 1 or helpPrintCurSensorValue doesn't work correctly
  static int staticVal2b[2]      = { 
    0xFFFF, 0xFFFF     }; // Need bit 16 to be 1 or helpPrintCurSensorValue doesn't work correctly
  byte maxChars                  = 17;  // Max number of characters in edit fields
  byte yAdj;

  setDefaultPins();  // clear camera/flash and sensor pins back to default (especially reset camera focus)

  if (g_mPos < 8)
    yAdj = 0;
  else
    yAdj = 27;

  printMenuTitle(_tx_00_advancedSensorMenu, maxChars, yAdj);
  printMenuName(_tx_00_device1Device2,      0, yAdj);
  printMenuName(_tx_00_trigSen,             1, yAdj);
  printMenuName(_tx_00_delayms,             2, yAdj);
  printMenuName(_tx_00_bulbSec,             3, yAdj);
  printMenuName(_tx_00_prefocus,            4, yAdj);
  printMenuName(_tx_00_sensor1Sensor2,      5, yAdj);
  printMenuName(_tx_00_trigType,            6, yAdj);
  printMenuName(_tx_00_trigVal,             7, yAdj);
  printMenuName(_tx_00_power,               8, yAdj);

  // Draw constant lines
  g_dogm.setVLine(41, 0, 53);
  g_dogm.setBox(42, 45+yAdj, 127, 45+yAdj);
  g_dogm.setBox(42, 8+yAdj, 127, 9+yAdj);
  g_dogm.setBox(42, 0+yAdj, 127, 0+yAdj);
  g_dogm.setBox(0, 45+yAdj, 40, 53+yAdj);
  g_dogm.setBox(0, 0+yAdj, 40, 9+yAdj);

  // Input current sensor values
  if ((eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR1) == SENSOR_EXP_S1) || (eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR2) == SENSOR_EXP_S1) ||
    (eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR1) >  SENSOR_EXP_S2) || (eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR2) >  SENSOR_EXP_S2))  // Only display if a device is using sensor1
    helpPrintCurSensorValue(58,  -17+yAdj, 0, eepromReadInt(EEPROM_ADV_SENSOR_TRIG_MODE1), staticVal1a, staticVal1b);

  if ((eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR1) >= SENSOR_EXP_S2) || (eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR2) >= SENSOR_EXP_S2))  // Only display if a device is using sensor2
      helpPrintCurSensorValue(103, -17+yAdj, 1, eepromReadInt(EEPROM_ADV_SENSOR_TRIG_MODE2), staticVal2a, staticVal2b);

  // Draw the changeable values in this menu (only one can be selected at a time)
  button |= printMenuSetString(maxChars,   1, g_mPos == 0,  EEPROM_ADV_DEVICE_TRIG_SENSOR1,   8, 5, _tx_00_sensorList, yAdj);
  button |= printMenuSetString(maxChars-9, 1, g_mPos == 1,  EEPROM_ADV_DEVICE_TRIG_SENSOR2,   8, 5, _tx_00_sensorList, yAdj);

  if (eepromReadInt(EEPROM_GENERAL_DELAY_DECIMALS) == 0) 
  {
    button |= printMenuSetNumber(maxChars,   2, g_mPos == 2,  EEPROM_ADV_DEVICE_DELAY1,       3, 1, 9, yAdj);
    button |= printMenuSetNumber(maxChars-9, 2, g_mPos == 3,  EEPROM_ADV_DEVICE_DELAY2,       3, 1, 9, yAdj);
  }
  else
  {
    button |= printMenuSetNumber(maxChars,   2, g_mPos == 2,  EEPROM_ADV_DEVICE_DELAY1,       1, 3, 9, yAdj);
    button |= printMenuSetNumber(maxChars-9, 2, g_mPos == 3,  EEPROM_ADV_DEVICE_DELAY2,       1, 3, 9, yAdj);
  }  


  button |= printMenuSetNumber(maxChars,   3, g_mPos == 4,  EEPROM_ADV_DEVICE_CYCLE1,         3, 1, 9, yAdj);
  button |= printMenuSetNumber(maxChars-9, 3, g_mPos == 5,  EEPROM_ADV_DEVICE_CYCLE2,         3, 1, 9, yAdj);
  button |= printMenuSetString(maxChars,   4, g_mPos == 6,  EEPROM_ADV_DEVICE_PREFOCUS1,      3, 2, _tx_00_noYesList, yAdj);
  button |= printMenuSetString(maxChars-9, 4, g_mPos == 7,  EEPROM_ADV_DEVICE_PREFOCUS2,      3, 2, _tx_00_noYesList, yAdj);
  button |= printMenuSetString(maxChars,   6, g_mPos == 8,  EEPROM_ADV_SENSOR_TRIG_MODE1,     8, 4, _tx_00_trigTypeList, yAdj);
  button |= printMenuSetString(maxChars-9, 6, g_mPos == 9,  EEPROM_ADV_SENSOR_TRIG_MODE2,     8, 4, _tx_00_trigTypeList, yAdj);

  if (eepromReadInt(EEPROM_ADV_SENSOR_TRIG_MODE1) == SENSOR_TRIGGER_DIGITAL)
    button |= printMenuSetString(maxChars,   7, g_mPos == 10, EEPROM_ADV_SENSOR_TRIG_VAL1,    3, 2, _tx_00_lowHighList, yAdj);
  else
    button |= printMenuSetNumber(maxChars,   7, g_mPos == 10, EEPROM_ADV_SENSOR_TRIG_VAL1,    3, 0, 9, yAdj);

  if (eepromReadInt(EEPROM_ADV_SENSOR_TRIG_MODE2) == SENSOR_TRIGGER_DIGITAL)
    button |= printMenuSetString(maxChars-9, 7, g_mPos == 11, EEPROM_ADV_SENSOR_TRIG_VAL2,    3, 2, _tx_00_lowHighList, yAdj);
  else
    button |= printMenuSetNumber(maxChars-9, 7, g_mPos == 11, EEPROM_ADV_SENSOR_TRIG_VAL2,    3, 0 , 9, yAdj);

  button |= printMenuSetString(maxChars,   8, g_mPos == 12, EEPROM_ADV_SENSOR_POWER1,         8, 3, _tx_00_powerList, yAdj);
  button |= printMenuSetString(maxChars-9, 8, g_mPos == 13, EEPROM_ADV_SENSOR_POWER2,         8, 3, _tx_00_powerList, yAdj);
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
        else                // is even
        ++g_mPos;
      }
      break;
    case BUTTON_UP:
      {
        if (g_mPos<=1)
          g_mPos += 12;
        else
          g_mPos -= 2;
      }
      break;
    case BUTTON_DOWN:
      {
        if (g_mPos>=12)
          g_mPos -= 12;
        else
          g_mPos += 2;
      }
      break;
    }
  }
}

////////////////////////////////////////
// Function: advancedSensorFunc - Handles camera/flash triggering for the generic sensor/flash menu.  Since each device can be
//                        triggered by any sensor this function acts as a crossbar
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////


void advancedSensorFunc()
{
  unsigned long deviceDelayTimes[2] = {  
    0,  0     };
  unsigned long deviceCycleTimes[2] = {  
    0,  0     };

  int prevSensorVals[2]          = { 
    -1, -1     };
  int eepromDeviceTrigSensors[2] = { 
    eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR1), eepromReadInt(EEPROM_ADV_DEVICE_TRIG_SENSOR2)     };
  int eepromSensorTriggerMode[2] = { 
    eepromReadInt(EEPROM_ADV_SENSOR_TRIG_MODE1),   eepromReadInt(EEPROM_ADV_SENSOR_TRIG_MODE2)     };
  int eepromSensorTrigVals[2]    = { 
    eepromReadInt(EEPROM_ADV_SENSOR_TRIG_VAL1),    eepromReadInt(EEPROM_ADV_SENSOR_TRIG_VAL2)     };
  int eepromDeviceDelays[2]      = { 
    eepromReadInt(EEPROM_ADV_DEVICE_DELAY1),       eepromReadInt(EEPROM_ADV_DEVICE_DELAY2)     };
  int eepromSensorPowers[2]      = { 
    eepromReadInt(EEPROM_ADV_SENSOR_POWER1),       eepromReadInt(EEPROM_ADV_SENSOR_POWER2)     };
  int eepromDeviceCycles[2]      = { 
    eepromReadInt(EEPROM_ADV_DEVICE_CYCLE1),       eepromReadInt(EEPROM_ADV_DEVICE_CYCLE2)     };
  int eepromDevicePrefocuses[2]  = { 
    eepromReadInt(EEPROM_ADV_DEVICE_PREFOCUS1),    eepromReadInt(EEPROM_ADV_DEVICE_PREFOCUS2)     };

  unsigned long curTime;            // micros() has a resolution of 4 microseconds

  int sensorVals[2];
  byte s;  // sensor number 0/1
  byte d;  // device number 0/1
  
  extern void helpSensorDevicePass (unsigned long curTime, byte s, byte d, unsigned long *deviceDelayTimes,
 unsigned long *deviceCycleTimes, int *sensorVals, int *prevSensorVals, int *eepromDeviceTrigSensors,
 int *eepromSensorTriggerMode, int *eepromSensorTrigVals, int *eepromDeviceDelays, int *eepromSensorPowers, 
 int *eepromDeviceCycles, int *eepromDevicePrefocuses); // added by DML1333 for compatibility with IDE 1.6.6+

  attachInterrupt(0, activeButtonISR, LOW);
  g_dogm.start();

  do 
  {
    printMenuTitle(_tx_00_advancedPhotoMode, 0xFF, 0); 
    printMenuName(_tx_00_pressActivateToExit, 5, 0);
  } 
  while( g_dogm.next() );

  if (eepromDevicePrefocuses[0])
    setDevicePins(0, HIGH, LOW);
  if (eepromDevicePrefocuses[1])
    setDevicePins(1, HIGH, LOW);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
  //
  // This begins the sensor testing loop for as long as in PHOTO mode
  //	

  while (g_menuMode == MENUMODE_PHOTO)
  {
    curTime = micros();    // micros() has a resolution of 4 microseconds
    int sensorVals[2] = { 
      -1,-1         };

    for(d=0; d<2; ++d)
    {
      for(s=0; s<2; ++s)
      {
        helpSensorDevicePass(curTime, s, d, deviceDelayTimes, deviceCycleTimes, sensorVals, prevSensorVals, 
        eepromDeviceTrigSensors, eepromSensorTriggerMode, eepromSensorTrigVals, eepromDeviceDelays, 
        eepromSensorPowers, eepromDeviceCycles, eepromDevicePrefocuses);   	
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
  //
  // Out of PHOTO mode now, so let the buttons settle down
  //
  buttonDebounce(10); // 10 ms debounce
  //
  // Now back to MENU mode  
}

////////////////////////////////////////
// Function: testDeviceForTrigger - This function checks to see if the current device is triggering
// Parameters:
//   s                              - Current sensor (either 0 or 1)
//   d                              - Current device (either 0 or 1)
//   deviceOR                       - true if the device is OR-ing the sensors together
//   deviceAND                      - true if the device is AND-ing the sensors together
//   sensorVals                     - Returns the value measured by sensor (array of size 2)
//   prevSensorVals                 - Returns the previous value measured by sensor (array of size 2)
//   eepromDeviceTrigSensors        - Copy of equivalent data in eeprom - improves perf
//   eepromSensorTriggerMode        - Copy of equivalent data in eeprom - improves perf
//   eepromSensorTrigVals           - Copy of equivalent data in eeprom - improves perf
//
// Returns:
//   See parameters: sensorVals, prevSensorVals
////////////////////////////////////////
boolean testDeviceForTrigger(byte s, byte d, boolean deviceOR, boolean deviceAND, int *sensorVals, int *prevSensorVals,
int *eepromDeviceTrigSensors, int *eepromSensorTriggerMode, int *eepromSensorTrigVals)
{
  byte i;
  boolean t = false;

  if (eepromSensorTriggerMode[d] == SENSOR_TRIGGER_DIGITAL) // Want to use a digital signal
  {
    byte trigVal;

    if (eepromSensorTrigVals[d] > 1)
      trigVal = HIGH;
    else
      trigVal = LOW;

    return (readSensorDigitalFast(d)==trigVal);
  }

  if (deviceOR || deviceAND)
  {
    if (s == 1)
      return false;    // Sensor 2 has already been taken into account if OR-ing or AND-ing

    for(i=0; i<2; ++i)
    {
      if (sensorVals[i] == -1)
        sensorVals[i] = readSensorAnalog(i);
    }
  }
  else
  {  
    if (sensorVals[s] == -1)
      sensorVals[s] = readSensorAnalog(s);
  }

  // Prevent threshold from instantly triggering on first try
  if (prevSensorVals[0] < 0)
    prevSensorVals[0] = sensorVals[0];
  if (prevSensorVals[1] < 0)
    prevSensorVals[1] = sensorVals[1];

  // Testing for (OR), (AND), (single active sensor)
  if (deviceOR || deviceAND)
  {
    boolean te[2] = { 
      false, false          };

    for (i=0; i<2; ++i)
    {
      if (((eepromSensorTriggerMode[i]==SENSOR_TRIGGER_LOW)      && (sensorVals[i] < eepromSensorTrigVals[i])) ||                   // Trigger on low signal
      ((eepromSensorTriggerMode[i]==SENSOR_TRIGGER_HIGH)     && (sensorVals[i] > eepromSensorTrigVals[i])) ||                       // Trigger on high signal
      ((eepromSensorTriggerMode[i]==SENSOR_TRIGGER_THRESHOLD) && (abs(sensorVals[i]-prevSensorVals[i]) > eepromSensorTrigVals[i]))) // Trigger on threshold
      {
        te[i] = true;
      }
    }
    if    (te[0] && te[1])       // If both are true, then deviceAND does not matter
      t = true;
    else if (deviceOR && (te[0] || te[1]))
      t = true;

  }
  else if (((eepromSensorTriggerMode[s]==SENSOR_TRIGGER_LOW)      && (sensorVals[s] < eepromSensorTrigVals[s])) ||              // Trigger on low signal
  ((eepromSensorTriggerMode[s]==SENSOR_TRIGGER_HIGH)     && (sensorVals[s] > eepromSensorTrigVals[s])) ||                       // Trigger on high signal
  ((eepromSensorTriggerMode[s]==SENSOR_TRIGGER_THRESHOLD) && (abs(sensorVals[s]-prevSensorVals[s]) > eepromSensorTrigVals[s]))) // Trigger on threshold
  {
    t = true;
  }

  return t;
}

////////////////////////////////////////
// Function: helpSensorDevicePass   - This function does all the work and sensorFunc gets the credit (see sensorFunc)
// Parameters:
//   curTime                        - The current time measured in microseconds
//   s                              - Current sensor (either 0 or 1)
//   d                              - Current device (either 0 or 1)
//   deviceDelayTimes               - Returns the microsecond time when device delay expires (array of size 2)
//   deviceCycleTimes               - Returns the microsecond time when device cycle expires (array of size 2)
//   sensorVals                     - Returns the value measured by sensor (array of size 2)
//   prevSensorVals                 - Returns the previous value measured by sensor (array of size 2)
//   eepromDeviceTrigSensors        - Copy of equivalent data in eeprom - improves perf
//   eepromSensorTriggerMode        - Copy of equivalent data in eeprom - improves perf
//   eepromSensorTrigVals           - Copy of equivalent data in eeprom - improves perf
//   eepromDeviceDelays             - Copy of equivalent data in eeprom - improves perf
//   eepromSensorPowers             - Copy of equivalent data in eeprom - improves perf
//   eepromDeviceCycles             - Copy of equivalent data in eeprom - improves perf
//   eepromDevicePrefocuses         - Copy of equivalent data in eeprom - improves perf
//
// Returns:
//   See parameters: deviceDelayTimes, deviceCycleTimes, sensorVals, prevSensorVals
////////////////////////////////////////

void helpSensorDevicePass(unsigned long curTime, byte s, byte d, unsigned long *deviceDelayTimes,
 unsigned long *deviceCycleTimes, int *sensorVals, int *prevSensorVals, int *eepromDeviceTrigSensors,
 int *eepromSensorTriggerMode, int *eepromSensorTrigVals, int *eepromDeviceDelays, int *eepromSensorPowers, 
 int *eepromDeviceCycles, int *eepromDevicePrefocuses)
{
  boolean deviceOR  = eepromDeviceTrigSensors[d] == SENSOR_EXP_S1_OR_S2;
  boolean deviceAND = eepromDeviceTrigSensors[d] == SENSOR_EXP_S1_AND_S2;


  // If DeviceX is using SensorX
  if ((s+1 == eepromDeviceTrigSensors[d]) || deviceOR || deviceAND)
  {
    if ( !deviceDelayTimes[d] && !deviceCycleTimes[d] ) // If not in delay or cycle
    {
      boolean t = testDeviceForTrigger(s, d, deviceOR, deviceAND, sensorVals, prevSensorVals, 
      eepromDeviceTrigSensors, eepromSensorTriggerMode, eepromSensorTrigVals);

      if (t)
      {
        deviceDelayTimes[d] = (unsigned long)curTime + ((unsigned long)eepromDeviceDelays[d] * g_Delay_Scaler);

        if (eepromSensorPowers[0] == d+1) // Off on sensor1
            setSensorPowerPin(0, LOW);
        if (eepromSensorPowers[1] == d+1) // Off on sensor2
          setSensorPowerPin(1, LOW);
      }
    }
    else 
    {
      // The third part of this check prevents bugs from a timer overflow
      if ( deviceDelayTimes[d] && (deviceDelayTimes[d] < curTime) && (curTime - deviceDelayTimes[d] < 1000000000) )
      {
        const unsigned long cycleTime = eepromDeviceCycles[d]*100000;

        setDevicePins(d, HIGH, HIGH);
        deviceDelayTimes[d] = 0;
        deviceCycleTimes[d] = curTime + cycleTime;
      }
      else if ( deviceCycleTimes[d] && (deviceCycleTimes[d] < curTime) && (curTime - deviceCycleTimes[d] < 1000000000) )
      {
        if (eepromDeviceCycles[d] == 0)
        {
          boolean t = testDeviceForTrigger(s, d, deviceOR, deviceAND, sensorVals, prevSensorVals, 
          eepromDeviceTrigSensors, eepromSensorTriggerMode, eepromSensorTrigVals);

          if (t)
            return; // For 0 cycle (bulb) times we only want to clear trigger when no longer triggered
        }
        if (eepromDevicePrefocuses[d])
          setDevicePins(d, HIGH, LOW);
        else
          setDevicePins(d, LOW, LOW);

        deviceCycleTimes[d] = 0;

        if (eepromSensorPowers[0] == d+1)  // Turn power back on
          setSensorPowerPin(0, HIGH);
        if (eepromSensorPowers[1] == d+1)  // Turn power back on
          setSensorPowerPin(1, HIGH);

        prevSensorVals[0] = -1;  // Reset so threshold doesn't trigger immediately on next pass
        prevSensorVals[1] = -1;  // Reset so threshold doesn't trigger immediately on next pass

        if (eepromDeviceCycles[d] != 0)
          delay(200);  // Lets us trigger retrigger but don't do this for cycle (bulb) times of 0
      }
    }
  }
}

////////////////////////////////////////
// Function: helpPrintCurSensorValue - This reads an analog pin and prints out the value to the LCD
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   y            - Y position on LCD to start drawing (bottom left is 0,0)
//   sensor       - The analog sensor you want to read from
//   triggerMode  - See the enum that defines the modes
//   valuesA      - An array of 2 values being updated. 2 values are needed so we can double buffer results.
//   valuesB      - Same as valuesA, but more storage needed for threshold
//
// Returns:
//   This updates the pass by reference "value" variable
////////////////////////////////////////

void helpPrintCurSensorValue(byte x, byte y, byte sensor, int triggerMode, int *valuesA, int *valuesB)
{
  int i;
  int sensorValMin;
  int sensorValMax;
  byte activeIndex, prevIndex;


  if (triggerMode == SENSOR_TRIGGER_DIGITAL)
  {
    // Use a much simpler path for digital signals where we just check ever time
    int sensorVal = readSensorDigitalFast(sensor);

    g_dogm.setXY(x, y);
    g_dogm.print("/");

    if (sensorVal == HIGH)
      g_dogm.print(getStr(_tx_00_hi));
    else
      g_dogm.print(getStr(_tx_00_low));

    return;
  }

  if ( (millis()%1000) < 500 )  // Update every 0.5 second
  {
    activeIndex = 0;
    prevIndex = 1;
  }
  else
  {
    activeIndex = 1;
    prevIndex = 0;
  }

  // This figures out the sensor value for a few quick checks
  sensorValMin = readSensorAnalog(sensor);
  sensorValMax = sensorValMin;
  for(i=0; i<5; ++i)
  {
    int tSensorVal = readSensorAnalog(sensor);
    sensorValMin = min(sensorValMin, tSensorVal);
    sensorValMax = max(sensorValMax, tSensorVal);
  }

  sensorValMin = min(sensorValMin, 999);  // Clamp to a max value of 999
  sensorValMax = min(sensorValMax, 999);

  // We use the msb of prevIndex to track whether we have reset the running total during
  // this half second period
  if (valuesA[activeIndex] & 0x8000)
  {
    valuesA[activeIndex] = sensorValMin;
    valuesA[prevIndex]   = valuesA[prevIndex] | 0x8000;
    valuesB[activeIndex] = sensorValMax;
    valuesB[prevIndex]   = valuesB[prevIndex] | 0x8000;
  }
  else
  {
    if      (triggerMode == SENSOR_TRIGGER_LOW)
      valuesA[activeIndex] = min(sensorValMin,  valuesA[activeIndex]);

    else if (triggerMode == SENSOR_TRIGGER_HIGH)
      valuesA[activeIndex] = max(sensorValMax,  valuesA[activeIndex]);

    else if (triggerMode == SENSOR_TRIGGER_THRESHOLD) {
      valuesA[activeIndex] = min(sensorValMin,  valuesA[activeIndex]);
      valuesB[activeIndex] = max(sensorValMin,  valuesB[activeIndex]);
    }

  }

  g_dogm.setXY(x, y);
  g_dogm.print("/");

  if (triggerMode == SENSOR_TRIGGER_THRESHOLD ) 
  {
    g_dogm.print((valuesB[prevIndex] & 0x7fff)-(valuesA[prevIndex] & 0x7fff));
  }
  else if (triggerMode == SENSOR_TRIGGER_THRSH_POS)
  {
    int dif = valuesB[prevIndex] - valuesA[prevIndex];
    dif = max(0, dif);
    g_dogm.print(dif);
  }
  else 
  {
    g_dogm.print(valuesA[prevIndex] & 0x7fff);
  }
}


#endif




