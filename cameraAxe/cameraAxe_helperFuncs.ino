////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Define digital/analog hardware pins
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
//#define HW_RX_PIN              0
//#define HW_TX_PIN              1
#define BUTTON_ACTIVATE_PIN    2
#define DEVICE1_SHUTTER_PIN    3
#define DEVICE1_FOCUS_PIN      4
#define DEVICE2_SHUTTER_PIN    5
#define DEVICE2_FOCUS_PIN      6
#define SENSOR2_POWER_PIN      7
#define LCD_POWER_PIN          8
#define LCD_A0_PIN             9
#define LCD_CS_PIN            10
#define LCD_SI_PIN            11
//(Reserved for LCD)          12
#define LCD_SCL_PIN           13
#define BUTTONS_APIN           0
#define BUTTONS_PIN           54
#define SENSOR1_POWER_PIN     55
#define SENSOR1_APIN           2
#define SENSOR1_PIN           56
#define SENSOR1_TIP_PIN       57
#define SENSOR2_APIN           4
#define SENSOR2_PIN           58
#define SENSOR2_TIP_PIN       59
#else
//#define HW_RX_PIN              0
//#define HW_TX_PIN              1
#define BUTTON_ACTIVATE_PIN    2
#define DEVICE1_SHUTTER_PIN    3
#define DEVICE1_FOCUS_PIN      4
#define DEVICE2_SHUTTER_PIN    5
#define DEVICE2_FOCUS_PIN      6
#define SENSOR2_POWER_PIN      7
#define LCD_POWER_PIN          8
#define LCD_A0_PIN             9
#define LCD_CS_PIN            10
#define LCD_SI_PIN            11
//(Reserved for LCD)          12
#define LCD_SCL_PIN           13
#define BUTTONS_APIN           0
#define BUTTONS_PIN           14
#define SENSOR1_POWER_PIN     15
#define SENSOR1_APIN           2
#define SENSOR1_PIN           16
#define SENSOR1_TIP_PIN       17
#define SENSOR2_APIN           4
#define SENSOR2_PIN           18
#define SENSOR2_TIP_PIN       19
#endif

////////////////////////////////////////
// Function: activeButtonIRS - The interupt service routine to handle pressing of the active button
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void activeButtonISR()
{
  detachInterrupt(0);
  g_menuMode = MENUMODE_MENU;
  buttonDebounce(10); // 10 ms debounce
}

////////////////////////////////////////
// Function: buttonDebounce - Test that no buttons are pressed for at least cont_time milliseconds 
// Parameters:
//   t - number of milliseconds of continuous no buttons pressed
// Returns:
//   None
////////////////////////////////////////
void buttonDebounce(int t)
{
   int count = t;

   while ( count > 0 )
   {
      count --;
      if ((analogRead(BUTTONS_APIN) <= 962 ) || (digitalRead(BUTTON_ACTIVATE_PIN) == LOW))
      {
         count = t;
      }
      delay(1);
   }
}

////////////////////////////////////////
// Function: getLcdPinA0 - Returns the LCD pin number
// Parameters:
//   None
// Returns:
//   Value of the LCD pin
////////////////////////////////////////
byte getLcdPinA0()
{
  return LCD_A0_PIN;
}

////////////////////////////////////////
// Function: readSensorAnalog - Reads the analog value from sensors
// Parameters:
//   s - 0 is Sensor1 and 1 is Sensor2
// Returns:
//   Analog value for that sensor
////////////////////////////////////////
int readSensorAnalog(byte s)
{
  return analogRead(SENSOR1_APIN + s*2); // takes about 100 microseconds
}

int readSensorAnalogTip(byte s)
{
  return analogRead(SENSOR1_APIN+1 + s*2); // takes about 100 microseconds
}



////////////////////////////////////////
// Function: readSensorDigital - Reads the digital value from sensors
// Parameters:
//   s - 0 is Sensor1 and 1 is Sensor2
// Returns:
//   Digital value for that sensor
////////////////////////////////////////
int readSensorDigital(byte s)
{
  return digitalRead(SENSOR1_PIN + s*2);
}

byte readSensorDigitalFast(byte s)  // 15 times faster than readSensorDigital()
{
  return bitRead(PINC, 2 + s*2);
}

////////////////////////////////////////
// Function: setSensorPinModes - Sets the pin mode for the different pins for each sensor
// Parameters:
//   sensor  - 0 is Sensor1 and 1 is Sensor2
//   mid     - INPUT or OUTPUT for the middle pin of the 3.5mm jack
//   tip     - INPUT or OUTPUT for the tip pin of the 3.5mm jack
// Returns:
//   None
////////////////////////////////////////
void setSensorPinModes(byte sensor, byte mid, byte tip)
{
  if (sensor == 0)
  {
    pinMode(SENSOR1_PIN,         mid);
    pinMode(SENSOR1_TIP_PIN,     tip);
  }
  else
  {
    pinMode(SENSOR2_PIN,         mid);
    pinMode(SENSOR2_TIP_PIN,     tip);
  }
}

////////////////////////////////////////
// Function: setAnalogReadSpeed - Switches between faster and slower analog read speeds
// Parameters:
//   x  - 0 is slower and 1 is faster
// Returns:
//   None
////////////////////////////////////////
void setAnalogReadSpeed(byte x)
{
    // ADPS2  ADPS1  ADPS0  Div Factor
    // 0      0      0      2
    // 0      0      1      2
    // 0      1      0      4
    // 0      1      1      8
    // 1      0      0      16
    // 1      0      1      32
    // 1      1      0      64
    // 1      1      1      128
    
    if (x == 1)
    {
      // Default set by wiring is 128 (16 MHz/128 = 125 Khz)
      // Since a converstion takes 13 clocks the sample rate is 125Khz/13 = 9600 Hz or about 100 us.
      // By setting the div factor to 16 analog reads will be 8 times faster.
      ADCSRA |=  (1<<ADPS2);
      ADCSRA &= ~(1<<ADPS1);
      ADCSRA &= ~(1<<ADPS0);
    }
    else if (x == 0)
    {
      // Set div factor to default of 128 for slower more accurate analog reads
      ADCSRA |= (1<<ADPS2);
      ADCSRA |= (1<<ADPS1);
      ADCSRA |= (1<<ADPS0);
    }
}

////////////////////////////////////////
// Function: setSensor - Sets the output of the sensors (only valid if sensor mode is OUTPUT)
// Parameters:
//   s   - 0 is Sensor1 and 1 is Sensor2
//   mid - Valid values are LOW or HIGH
//   tip - Valid values are LOW or HIGH
// Returns:
//   None
////////////////////////////////////////
void setSensorPins(byte s, byte mid, byte tip)
{
  digitalWrite(SENSOR1_PIN+s*2, mid);
  digitalWrite(SENSOR1_TIP_PIN+s*2, tip);
}

////////////////////////////////////////
// Function: setSensorPowerPin - Sets the output of the sensors (only valid if sensor mode is OUTPUT)
// Parameters:
//   s   - 0 is Sensor1 and 1 is Sensor2
//   pow - Valid values are LOW or HIGH
// Returns:
//   None
////////////////////////////////////////
void setSensorPowerPin(byte s, byte pow)
{
  digitalWrite(SENSOR1_POWER_PIN-s*8,  !pow);
}

////////////////////////////////////////
// Function: lcdPower - Turns the LCD backlight off/on
// Parameters:
//   val - Valid values are LOW or HIGH
// Returns:
//   None
////////////////////////////////////////
void lcdPower(byte val)
{
  digitalWrite(LCD_POWER_PIN, val);
}

////////////////////////////////////////
// Function: setDevicePins - Triggers and untriggers a device (camera/flash)
// Parameters:
//   device  - 0 is Device1 and 1 is Device2
//   focus   - Valid values are LOW or HIGH
//   Shutter - Valid values are LOW or HIGH
// Returns:
//   None
////////////////////////////////////////
void setDevicePins(byte device, byte focus, byte shutter)
{
#ifdef USE_IRREMOTEMENU
  if(eepromReadInt(EEPROM_DEVICE1_TYPE - 1 + device) != WIRED)
  {
    sendFocusSignal(device, DEVICE1_FOCUS_PIN+device*2, focus);
    sendShutterSignal(device, DEVICE1_SHUTTER_PIN+device*2, shutter);
  }
  else
  {
    digitalWrite(DEVICE1_FOCUS_PIN+device*2, focus);
    digitalWrite(DEVICE1_SHUTTER_PIN+device*2, shutter);
  }
#else
  digitalWrite(DEVICE1_FOCUS_PIN+device*2, focus);
  digitalWrite(DEVICE1_SHUTTER_PIN+device*2, shutter);
#endif
}

////////////////////////////////////////
// Function: testMode - Runs a test to verify things are working as expected using a test jig
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void testMode()
{
  while (detectButtonPress(false) == BUTTON_ACTIVATE)
  {
    // Blink sensor LEDs on with transistor
    setSensorPinModes(0, OUTPUT, INPUT);
    setSensorPinModes(1, OUTPUT, INPUT);
    setSensorPins(0, HIGH, LOW);
    setSensorPins(1, HIGH, LOW);
    setSensorPowerPin(0, HIGH);
    setSensorPowerPin(1, HIGH);
    delay(400);

    // Blink sensor LEDs off
    setSensorPinModes(0, OUTPUT, OUTPUT);
    setSensorPinModes(1, OUTPUT, OUTPUT);
    setSensorPins(0, LOW, LOW);
    setSensorPins(1, LOW, LOW);
    setSensorPowerPin(0, LOW);
    setSensorPowerPin(1, LOW);
    delay(400);

    // Blink sensor LEDs on and device LEDs on (transistors off)
    setSensorPins(0, HIGH, HIGH);
    setSensorPins(1, HIGH, HIGH);
    setDevicePins(0, HIGH, HIGH);
    setDevicePins(1, HIGH, HIGH);
    setSensorPowerPin(0, LOW);
    setSensorPowerPin(1, LOW);
    delay(400);

    // Blink sensor LEDs off
    setDevicePins(0, LOW, LOW);
    setDevicePins(1, LOW, LOW);
    setSensorPins(0, LOW, LOW);
    setSensorPins(1, LOW, LOW);
    setSensorPowerPin(0, LOW);
    setSensorPowerPin(1, LOW);
    delay(400);
  }
}


////////////////////////////////////////
// Function: setupCameraAxePins - Sets up the initial values of the camera axe hw pins
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void setupCameraAxePins()
{
  // Setup input/output on all pins being used as digital in/outs
  pinMode(BUTTON_ACTIVATE,     INPUT);
  pinMode(DEVICE1_SHUTTER_PIN, OUTPUT);
  pinMode(DEVICE1_FOCUS_PIN,   OUTPUT);
  pinMode(DEVICE2_SHUTTER_PIN, OUTPUT);
  pinMode(DEVICE2_FOCUS_PIN,   OUTPUT);
  pinMode(LCD_POWER_PIN,       OUTPUT);
  // All the LCD pins are setup in the DogmCA class
  
  setDefaultPins();

  digitalWrite(BUTTONS_PIN, HIGH);         // Turn on 20K pullup
  digitalWrite(BUTTON_ACTIVATE_PIN, HIGH);
  //digitalWrite(SENSOR1_PIN, HIGH);
  //digitalWrite(SENSOR2_PIN, HIGH);
}

////////////////////////////////////////
// Function: setDefaultPins - Sets up the initial values of the camera axe pins
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void setDefaultPins()
{
  setSensorPinModes(0, INPUT, INPUT);  // INPUT explicitly disables any menus that might have been setting INPUT_PULLUP 
  setSensorPinModes(1, INPUT, INPUT);  // INPUT explicitly disables any menus that might have been setting INPUT_PULLUP
  pinMode(SENSOR1_POWER_PIN, OUTPUT);
  pinMode(SENSOR2_POWER_PIN, OUTPUT);
  setSensorPowerPin(0, HIGH);
  setSensorPowerPin(1, HIGH);
  setSensorPins(0, LOW, LOW);
  setSensorPins(1, LOW, LOW);
  setDevicePins(0, LOW, LOW);
  setDevicePins(1, LOW, LOW);
}

////////////////////////////////////////
// Function: lcdSetString - Sets a string using buttons and the LCD
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   y            - Y position on LCD to start drawing (bottom left is 0,0)
//   selected     - The current value should be selected (ie xor box drawn around it)
//   edit         - If true we should enter edit mode for this value
//   numStrings   - The number of strings in the list of string ("strings")
//   stringLength - The lenght of a string (all strings should be the same length)
//   eepromPos    - Location in the list of values stored in the eeprom
//   strings      - A list of strings that the user can select from
// Returns:
//   Last button pressed which caused us to exit this function
////////////////////////////////////////
byte lcdSetString(byte x, byte y, boolean selected, boolean edit, byte numStrings, byte stringLength, byte eepromPos, const char *strings)
{
  byte button;
  byte val = eepromReadInt(eepromPos);
  
  if (val > numStrings)
  {
     // For the ditial/analog threshold case we can run into values too big
     val = numStrings-1;
  }
  
  if (!selected)
  {
    // If this value is not selected nothing can change so just print out the value
    g_dogm.setXY(x, y);
    g_dogm.print(getStr(strings+(stringLength+1)*val));
    return BUTTON_NONE;
  }
  
  button = detectButtonPress(true);
  
  if (edit)  // If in edit mode see if anything changed
  {
    switch (button)
    {
      case BUTTON_NONE:
      case BUTTON_MENU:
      case BUTTON_SELECT:
      case BUTTON_LEFT:
      case BUTTON_RIGHT:
      case BUTTON_CAM1:
      case BUTTON_CAM2:
      case BUTTON_ACTIVATE:
          break;  // Do nothing
      case BUTTON_UP:
        {
          if (val == 0)
            val = numStrings - 1;
          else
            --val;
          eepromWriteInt(eepromPos, val);
        }
        break;
      case BUTTON_DOWN:
        {
          val = (val+1)%numStrings;
          eepromWriteInt(eepromPos, val);
        }
        break;
    }     
  }
  
  g_dogm.setXY(x, y);
  //g_dogm.print(strings[val]);
  g_dogm.print(getStr(strings+(stringLength+1)*val));

  // xor box if not editing or blink the changing string in edit mode
  if ( !edit || (millis()%800) < 400 ) // Blink every 800 ms
  {
    g_dogm.xorBox(x, y, x+stringLength*g_dogm.charWidth-1, y+g_dogm.charHeight-1);
  }
  
  return button;
}


////////////////////////////////////////
// Function: lcdSetNumber - Sets a number using buttons and the LCD (up to increase number, down decrease number, left
//     to move to left digit, right to move to right digit) (numbers wrap from 9->0 and 0->9)
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   y            - Y position on LCD to start drawing (bottom left is 0,0)
//   selected     - The current value should be selected (ie xor box drawn around it)
//   edit         - If true we should enter edit mode for this value
//   cursorPos    - Position of the cursor should be between 0 (0 is all the way to the right) - (modified value is returned by reference)
//   eepromPos    - Location in the list of values stored in the eeprom
//   maxDigit     - Max most significant digit (useful for time where you only want to allow 59 seconds)
//   numCharsDec  - Number of chars in this number before the decimal
//   numCharsFrac - Number of chars in this number after decimail (if this is 0 then the decimal is not displayed)
// Returns:
//   Last button pressed which caused us to exit this function
////////////////////////////////////////
byte lcdSetNumber(byte x, byte y, boolean selected, boolean edit, byte *cursorPos, byte eepromPos, byte maxDigit, byte numCharsDec, byte numCharsFrac)
{
  int val = eepromReadInt(eepromPos);
  unsigned int bcdVal = decimalToBcd(val);
  byte button;
  byte adjNumChars = (numCharsFrac) ? (numCharsDec+numCharsFrac+1) : numCharsDec;
  const unsigned int bcdClearMask[4] = {0xFFF0, 0xFF0F, 0xF0FF, 0x0FFF};
  
  if (!selected)
  {
    lcdPrintZeros(x, y, bcdVal, numCharsDec, numCharsFrac);
    return BUTTON_NONE;
  }

  *cursorPos = min(*cursorPos, adjNumChars-1); // Clamp cursor position

  button = detectButtonPress(true);

  if (edit)
  {
    switch (button)
    {
      case BUTTON_NONE:
      case BUTTON_MENU:
      case BUTTON_SELECT:
      case BUTTON_CAM1:
      case BUTTON_CAM2:
      case BUTTON_ACTIVATE:
          break;  // Do nothing
      case BUTTON_UP:
      case BUTTON_DOWN:
        {
          unsigned int tempBcd = bcdVal;
          byte adjCursorPos;  
          
          if (numCharsFrac && (*cursorPos > numCharsFrac))
            adjCursorPos = (*cursorPos-1);
          else
            adjCursorPos = *cursorPos;

          tempBcd = (tempBcd >> (adjCursorPos*4)) & 0xF;  // Isolate single digit in BCD number that is changing

          if (button == BUTTON_UP)
          {
            if ((tempBcd == maxDigit) && (adjCursorPos == adjNumChars-1))
              tempBcd = 0;
            else if (tempBcd == 9)
              tempBcd = 0;
            else
              ++tempBcd;
          }
          else // BUTTON_DOWN
          {
            if ((tempBcd == 0) && (adjCursorPos == adjNumChars-1))
              tempBcd = maxDigit;
            else if (tempBcd == 0)
              tempBcd = 9;
            else
              --tempBcd;
          }

          bcdVal = (bcdVal & bcdClearMask[adjCursorPos]) | (tempBcd << (adjCursorPos*4));  // Mask out BCD character changing and replace with new
          val = bcdToDecimal(bcdVal);
          eepromWriteInt(eepromPos, val);
       }
        break;
      case BUTTON_LEFT:
        {
          *cursorPos = (*cursorPos + 1) % adjNumChars;
          if (numCharsFrac && (*cursorPos == numCharsFrac))  // special - adjusts for decimal point
            ++(*cursorPos);
        }
        break;
      case BUTTON_RIGHT:
        {
          if (*cursorPos == 0)
            *cursorPos = adjNumChars-1;
          else
            --(*cursorPos);

          if (numCharsFrac && (*cursorPos == numCharsFrac))  // special - adjusts for decimal point
            --(*cursorPos);
        }
        break;
    }
  }

  lcdPrintZeros(x, y, bcdVal, numCharsDec, numCharsFrac);
  
  // Invert a box around this number box
  g_dogm.xorBox(x, y, x+g_dogm.charWidth*adjNumChars-1, y+g_dogm.charHeight-1);
  
  // Flip the selected char again
  if (edit && (millis()%800) < 400 ) // Blink every 800 ms
  {
    byte xBlink = x + (adjNumChars - (*cursorPos) - 1) * g_dogm.charWidth;
    g_dogm.xorBox(xBlink, y, xBlink+g_dogm.charWidth-1, y+g_dogm.charHeight-1);
  }

  return button;
}

////////////////////////////////////////
// Function: lcdPrintZeros - Prints a number to the LCD and fills in empty digits with zeros
// Parameters:
//   x            - X position on LCD to start drawing (bottom left is 0,0)
//   y            - Y position on LCD to start drawing (bottom left is 0,0)
//   bcdVal       - The value being displayed, it must be passed in as a BCD (binary coded decimal)
//   numDigits    - Number of digits being printed (4 is the max) (ex:: bcdVal-45 and numDigit-3 would print 045)
//   numDigitsDec  - Number of digits in this number before the decimal (total digits max is 4 not counting a decimal) (ex:: bcdVal-45 and numDigit-3 would print 045)
//   numDigitsFrac - Number of digits in this number after decimal (if this is 0 then the decimal is not displayed)
// Returns:
//   None
////////////////////////////////////////
void lcdPrintZeros(byte x, byte y, unsigned int bcdVal, byte numDigitsDec, byte numDigitsFrac)
{
  byte i;
  unsigned int tempBcd;
  byte numDigits = numDigitsDec + numDigitsFrac;

  g_dogm.setXY(x,y);

  for(i=0; i<numDigits; ++i)
  {
    tempBcd = bcdVal >> (numDigits-1-i)*4;
    g_dogm.print(tempBcd & 0xF);
    
    if (numDigitsFrac && (i == (numDigitsDec-1)))
      g_dogm.print(".");
  }
}

////////////////////////////////////////
// Function: detectButtonPress - Figures out which button was pressed and does ss debounce
// Parameters:
//   waitForRelease - Wait until the button is released and do debounce
// Returns:
//   The button that was pressed
//
// DETAILED DESCRIPTION BELOW
//
// To allow multiple buttons to work off a single pin the Camera Axe attaches
// 6 buttons with some resistors in this configuration.
//           1K       1K        1K        1K        1K        1K
//  5V____/\/\/\____/\/\/\____/\/\/\____/\/\/\____/\/\/\____/\/\/\____/\/\/\____/\/\/\____GND
//                |         |         |         |         |         |         |         |
//                |         |         |         |         |         |         |         |
//                  /         /         /         /         /         /         /         /
//                 /         /         /         /         /         /         /         /
//                |         |         |         |         |         |         |         |
//                |         |         |         |         |         |         |         |
// BUTTONS_PIN---------------------------------------------------------------------------
//
// During init we set the button pin using a 20K internal pullup resistor
// in the microcontroller so ths signal isn't floating when no buttons are
// pressed.  Using some simple parallel/serial resistor and voltage
// divider equations I get get these ideal values:
//
// EX (first Equation): (((1K*20K)/(1K+20K))/(((1K*20K)/(1K+20K)+7000)))*5
//
// No button pressed:    5.0000 V  =  1023  [range          >962]
// Menu button pressed:  4.4012 V  =  900   [range <=962 && >843]
// Set button pressed:   3.8372 V  =  785   [range <=843 && >729]
// Up button pressed:    3.2857 V  =  672   [range <=729 && >615]
// Down button pressed:  2.7273 V  =  558   [range <=615 && >498]
// Left button pressed:  2.1429 V  =  438   [range <=498 && >374]
// Right button pressed: 1.5116 V  =  309   [range <=374 && >237]
// Cam1 button pressed:  0.8084 V  =  165   [range <=237 && >82]
// Cam2 button pressed:  0.0000 V  =  0     [range <=82        ]
//
////////////////////////////////////////
byte detectButtonPress(boolean waitForRelease)
{
  int buttonVal[3];
  byte button[3];
  byte i;

  buttonVal[0] = analogRead(BUTTONS_APIN);
  buttonVal[1] = analogRead(BUTTONS_APIN);
  buttonVal[2] = analogRead(BUTTONS_APIN);

  for(i=0; i<3; ++i)
  {
    if (buttonVal[i] > 962)
      button[i] = BUTTON_NONE;
    else if (buttonVal[i] <= 962 && buttonVal[i] > 843)
      button[i] = BUTTON_MENU;
    else if (buttonVal[i] <= 843 && buttonVal[i] > 729)
      button[i] = BUTTON_SELECT;
    else if (buttonVal[i] <= 729 && buttonVal[i] > 615)
      button[i] = BUTTON_UP;
    else if (buttonVal[i] <= 615 && buttonVal[i] > 498)
      button[i] = BUTTON_DOWN;
    else if (buttonVal[i] <= 498 && buttonVal[i] > 374)
      button[i] = BUTTON_LEFT;
    else if (buttonVal[i] <= 374 && buttonVal[i] > 237)
      button[i] = BUTTON_RIGHT;
    else if (buttonVal[i] <= 237 && buttonVal[i] > 82)
      button[i] = BUTTON_CAM1;
    else //(buttonVal <= 82)
      button[i] = BUTTON_CAM2;
  }
  
  // If 3 button readings don't match just go with none (happens during debounce)
  if ((button[0] != button[1]) || (button[0] != button[2]))
    return BUTTON_NONE;

  if (button[0] == BUTTON_CAM1)
  {
    setDevicePins(0, HIGH, HIGH);  // Trigger Camera1
    while(buttonVal[0] <= 237 && buttonVal[0] > 82)
      buttonVal[0] = analogRead(BUTTONS_APIN);
    setDevicePins(0, LOW, LOW);
  }
  else if (button[0] == BUTTON_CAM2)
  {
    setDevicePins(1, HIGH, HIGH);  // Trigger Camera2
    while(buttonVal[0] <= 82)
      buttonVal[0] = analogRead(BUTTONS_APIN);
    setDevicePins(1, LOW, LOW);
  }

    
  if (digitalRead(BUTTON_ACTIVATE_PIN) == LOW)
    button[0] = BUTTON_ACTIVATE;

  if ((button[0] != BUTTON_NONE) && waitForRelease)
  {
    // Spin waiting for the button to be released
    while ( (analogRead(BUTTONS_APIN) <= 941 ) || (digitalRead(BUTTON_ACTIVATE_PIN) == LOW) )
    {
      delay(1);
    }

    buttonDebounce(10);  // 10 ms debounce
  }
  
  if ((button[0] != BUTTON_NONE) && (eepromReadInt(EEPROM_GENERAL_LCD_BACKLIGHT) == 2))
    g_backlightMs = millis() + 10000; // 10 seconds

  return button[0];
}

////////////////////////////////////////
// Function: decimalToBcd - Converts a decimal to a BCD (binary coded decimal)
// Parameters:
//   dec - The decimal number
// Returns:
//   The BCD number
////////////////////////////////////////
unsigned int decimalToBcd(unsigned int dec)
{
  unsigned int bcd = 0;
  unsigned int i = 0;

  while (dec)
  {
    bcd += ((dec % 10)<<i);   // Convert lowest-order number
    i += 4;
    dec /= 10;
  }

  return(bcd);
}

////////////////////////////////////////
// Function: bcdToDecimal - Converts a BCD (binary coded decimal) to a decimal
// Parameters:
//   bcd - The bcd number
// Returns:
//   The decimal number
////////////////////////////////////////
unsigned int bcdToDecimal(unsigned int bcd)
{
  unsigned int dec = 0;
  unsigned int i = 1;

  while(bcd)
  {
    dec += (bcd & 0xF)*i;
    i *= 10;
    bcd = bcd >> 4;
  }

  return(dec);
}

////////////////////////////////////////
// Function: eepromWriteInt -  Writes an integer (16 bits) to eeprom
// Parameters:
//   addr - The address in eeprom to write to
//   val  - The value being written
// Returns:
//   None
////////////////////////////////////////
void eepromWriteInt(int addr, int val)
{
  int offset;
 
  // EEPROM_GENERAL_PRESET_GROUP, EEPROM_SIZE_PREVIOUS, and EEPROM_ACTIVE_MENU shouldn't change with presets so always read it from first preset
  if ((addr == EEPROM_GENERAL_PRESET_GROUP) || (addr == EEPROM_ACTIVE_MENU) || (addr == EEPROM_SIZE_PREVIOUS))
    offset = 0;
  else
    offset = eepromReadInt(EEPROM_GENERAL_PRESET_GROUP)*EEPROM_SIZE * 2;
  
  addr *= 2;  // int is 2 bytes
  addr += offset;
  EEPROM.write(addr+1, val&0xFF);
  val /= 256;
  EEPROM.write(addr+0, val&0xFF);
}

////////////////////////////////////////
// Function: eepromWriteInt -  Reads an integer (16 bits) from eeprom
// Parameters:
//   addr - The address in eeprom to read from
// Returns:
//   The value being read from eeprom
////////////////////////////////////////
int eepromReadInt(int addr)
{
  int val;
  int offset;
  
  // EEPROM_GENERAL_PRESET_GROUP, EEPROM_SIZE_PREVIOUS, and EEPROM_ACTIVE_MENU shouldn't change with presets so always read it from first preset
  if ((addr == EEPROM_GENERAL_PRESET_GROUP) || (addr == EEPROM_ACTIVE_MENU) || (addr == EEPROM_SIZE_PREVIOUS))
    offset = 0;
  else
    offset = eepromReadInt(EEPROM_GENERAL_PRESET_GROUP)*EEPROM_SIZE * 2;

  addr *= 2;  // int is 2 bytes
  addr += offset;
  val = EEPROM.read(addr+0);
  val *= 256;
  val |= EEPROM.read(addr+1);
  return val;
}

////////////////////////////////////////
// Function: eepromClamp -  Clamps the value in eeprom to the range given
// Parameters:
//   addr - The address in eeprom to read from
//   minVal  - Value being read clamps to this
//   maxVal  - Value being read clamps to this
// Returns:
//   The value being read from eeprom
////////////////////////////////////////
void eepromClamp(int addr, int minVal, int maxVal)
{
  int val1, val2;
  
  val1 = eepromReadInt(addr);
  val2 = constrain(val1, minVal, maxVal);
  if (val1 != val2)
    eepromWriteInt(addr, val2);
}

////////////////////////////////////////
// Function: menuProcessButton -  Most menus need to process certain buttons the same way, this handles that processing of button presses
// Parameters:
//   button - The byton press type
// Returns:
//   None
////////////////////////////////////////
void menuProcessButton(byte button)
{
  switch (button)
  {
    case BUTTON_NONE:
      break;  // Do nothing
    case BUTTON_MENU:
      {
        eepromWriteInt(EEPROM_ACTIVE_MENU, (eepromReadInt(EEPROM_ACTIVE_MENU)+1)%MENU_MAX_SIZE);
        g_editVal = 0;
        g_mPos = 0;
        break;
      }
    case BUTTON_ACTIVATE:
      {
        g_menuMode = (g_menuMode == MENUMODE_MENU) ? MENUMODE_PHOTO : MENUMODE_MENU;
        g_editVal = 0;
      }
      break;
    case BUTTON_SELECT:
      {
        g_editVal = !g_editVal;
        g_cursorPos = 4;
      }
      break;
  }
}

////////////////////////////////////////
// Function: nanoSec -  Returns the nanosecond count (similar to micro, but more precission and 4 times faster)
// Parameters:
//   None
// Returns:
//   Number of micros seconds in timer0
////////////////////////////////////////
extern volatile unsigned long timer0_overflow_count;
unsigned long nanoSec()
{
  return (((timer0_overflow_count << 8) + TCNT0)*500);
}

////////////////////////////////////////
// Function: resetTimer0 - Sets timer0 to 0
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void resetTimer0()
{
  TCNT0 = 0;
  timer0_overflow_count = 0;
}

////////////////////////////////////////
// Function: startNanoSec - Call before nanoSec() - This forces timer0 scaling to 1
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void startNanoSec()
{
  TCCR0B = 0x02;  // Force clock scaling to 1
}

////////////////////////////////////////
// Function: startNanoSec - Restores timer0 scaling to Arduino's 64 default (needed for all Arduino timing fucntions like delay, micro, milli,...)
// Parameters:
//   None
// Returns:
//   None
////////////////////////////////////////
void endNanoSec()
{
  TCCR0B = 0x03;  // Restore clock scaling to 64 (what Arduino expects)
}

///////////////////////////////////////
// Function: printMenuTitle - Centers words and prints line under title and draws vert line
// Parameters:
//   titleStr - The string of chars
//   maxChars - The max chars used in edit (this determines where to draw vert line
//   yAdj - Adjustment for scrolling text
// Returns:
//   None
////////////////////////////////////////
void printMenuTitle(const char *titleStr, byte maxChars, byte yAdj)
{
  byte charOffset = (128 -  g_dogm.charWidth * strlen(getStr(titleStr)))/2;
 
  g_dogm.setXY(charOffset, 56+yAdj);
  g_dogm.print(getStr(titleStr));
  
  if (maxChars == 0xFF)
    g_dogm.xorBox(0, 54, 127, 63);
  else
  {
    g_dogm.setBox(0, 54+yAdj, 127, 55+yAdj);
    g_dogm.setVLine(128 - maxChars * g_dogm.charWidth - 2, 0, 54+yAdj);
  }
}

///////////////////////////////////////
// Function: printMenuName - Prints name left justified
// Parameters:
//   nameStr - The string of chars
//   row     - Used to calculate y position
//   yAdj    - Adjustment for scrolling text
// Returns:
//   None
////////////////////////////////////////
void printMenuName(const char *nameStr, byte row, byte yAdj)
{
    byte y = 46 - row * (g_dogm.charHeight+1) + yAdj;
    g_dogm.setXY(0, y);
    g_dogm.print(getStr(nameStr));
}

///////////////////////////////////////
// Function: printMenuTitle2 - Prints name left justified
// Parameters:
//   nameStr - The string of chars
//   row     - Used to calculate y position
//   yAdj    - Adjustment for scrolling text
// Returns:
//   None
////////////////////////////////////////
void printMenuName2(const char *prefix, const char *nameStr, byte row, byte yAdj)
{
    byte y = 46 - row * (g_dogm.charHeight+1) + yAdj;
    g_dogm.setXY(0, y);
    g_dogm.print(prefix);
    g_dogm.print(getStr(nameStr));
}

////////////////////////////////////////
// Function: printMenuSetNumber - Sets a number using buttons and the LCD (up to increase number, down decrease number, left
//     to move to left digit, right to move to right digit) (numbers wrap from 9->0 and 0->9)
// Parameters:
//   maxChars     - Used to position text (left justified)
//   row          - Used to calculate y position
//   selected     - The current value should be selected (ie xor box drawn around it)
//   eepromPos    - Location in the list of values stored in the eeprom
//   numCharsDec  - Number of chars in this number before the decimal
//   numCharsFrac - Number of chars in this number after decimal (if this is 0 then the decimal is not displayed)
//   maxDigit     - Max most significant digit (useful for time where you only want to allow 59 seconds)
//   yAdj         - Adjustment for scrolling text
// Returns:
//   Last button pressed which caused us to exit this function
////////////////////////////////////////
byte printMenuSetNumber(byte maxChars, byte row, boolean selected, byte eepromPos, byte numCharsDec, byte numCharsFrac, byte maxDigit, byte yAdj)
{
  byte x = 128-maxChars*g_dogm.charWidth;
  byte y = 46 - row * (g_dogm.charHeight+1) + yAdj;
  return lcdSetNumber(x, y, selected, g_editVal, &g_cursorPos, eepromPos, maxDigit, numCharsDec, numCharsFrac);
}

////////////////////////////////////////
// Function: printMenuSetString - Sets a string using buttons and the LCD
// Parameters:
//   maxChars     - Used to position text (left justified)
//   row          - Used to calculate y position
//   selected     - The current value should be selected (ie xor box drawn around it)
//   eepromPos    - Location in the list of values stored in the eeprom
//   numChars     - Number of chars in this number (special case of 5 means 4+1 decimal place; 6 means 5+1 decimal place; 99 means 2+1 decimal place;)
//   numStrings   - The number of strings in the list of string ("strings")
//   strings      - A list of strings that the user can select from
//   yAdj         - Adjustment for scrolling text
// Returns:
//   Last button pressed which caused us to exit this function
////////////////////////////////////////
byte printMenuSetString(byte maxChars, byte row,boolean selected, byte eepromPos, byte numChars, byte numStrings, const char *strings, byte yAdj)
{
  byte x = 128-maxChars*g_dogm.charWidth;
  byte y = 46 - row * (g_dogm.charHeight+1) + yAdj;
  return lcdSetString(x, y, selected, g_editVal, numStrings, numChars, eepromPos, strings);
}

////////////////////////////////////////
// Function: menuScroll - Handles simple scrolling used by most menus
// Parameters:
//   button - Current button press
//   items  - Number of items being scrolled through
// Returns:
//   None
////////////////////////////////////////
void menuScroll(byte button, byte items)
{
  --items;
  if (!g_editVal)
  {
    switch (button)
    {
      case BUTTON_UP:
        {
          if (g_mPos == 0)
            g_mPos = items;
          else
            --g_mPos;
        }
        break;
      case BUTTON_DOWN:
        {
          if (g_mPos == items)
            g_mPos = 0;
          else
            ++g_mPos;
        }
        break;
    }
  }  
}

////////////////////////////////////////
// Function: stepMotor - Walks through 4 states of a 2 wire stepper motor
// Parameters:
//   motorNum - The motor number being controlled (0 = motor 1, and 1 = motor 2)
//   stepPos  - Current state of stepper motor
// Returns:
//   None
////////////////////////////////////////
void stepMotor(byte motorNum, byte stepPos)
{
  switch (stepPos)
  {
    case 0:
      setSensorPins(motorNum, LOW, HIGH);
      break;
    case 1:
      setSensorPins(motorNum, HIGH, HIGH);
      break;
    case 2:
      setSensorPins(motorNum, HIGH, LOW);
      break;
    case 3:
      setSensorPins(motorNum, LOW, LOW);
      break;
  }
}

////////////////////////////////////////
// Function: runStacker - Takes in motor params and runs them
// Parameters:
//   mSpeed   - Speed of the motor in revolations per minute
//   m1Dir    - Direction of motor 1
//   m2Dir    - Direciton of motor 2
//   m1Steps  - Number of steps for motor 1 before it stops (1 step = 2 degrees)
//   m2Steps  - Number of steps for motor 2 before it stops (1 step = 2 degrees)
// Returns:
//   None
////////////////////////////////////////
void runStackerMotor(int whichMotor, int mSpeed, int mDir, int mSteps)
{
  unsigned long mLastStepTime   = 0;
  unsigned long mDelayMs        = 60000L / (180 * mSpeed);  // 60,000 = 1000 ms/sec * 60 sec/min
  static byte mStepPos = 0;
  
  while ((g_menuMode != MENUMODE_MENU) && (mSteps != 0))
  {
    if (mSteps && (millis() - mLastStepTime >= mDelayMs))
    {
      mLastStepTime = millis();
      stepMotor(whichMotor, mStepPos);
      if (mDir)
      {
        if (mStepPos == 0)
          mStepPos = 3;
        else
          --mStepPos;
      }
      else
        mStepPos = (mStepPos + 1)%4;

      --mSteps;
    }
  }
}


