  Developer's Guide V1.1 for CameraAxe 5.XX

This document is an attempt to help guide developers in adding and modifying
code for the CameraAxe. Once the code has been tested/debugged thoroughly,
Maurice can decide if/when/how to integrate it into an official release. It
can then be moved out of the 'Developer' areas into the main code. There are
2 main sections. The first is related to adding new menus. The second is
related to modifying existing code.

After making any changes you should run the Arduino IDE and select
Files->Preferences and check the box 'Show verbose output during compilation'
and fix any errors or warnings that you can, that are related to your code.
This will help assure that the code meets the C++ language standards now and in the future.

To add a new menu....

  Include a #define USE_NEWMENU in the 'Start of User Selectable menus' so
    so that users can select to add/delete the features want. The characters
    'NEW' should be replaced with your menu name.

  If the new menu depends on some other menu being selected... or is
    mutually exclusive of other menu items. Include #ifdefs or other
    preprocessor statements that require/prevent inclusion in the area
    just below the user selectable items
      #ifdef USE_NEWMENU
        #define USE_requiredMENU
      #endif
    where 'required' is replaced by the required menu name.

  If the new menu requires another library, add the library just below the
    existing #includes

  If the new menu requires other #defines or new global variables include
    them in the section labeled 'Start developer #defines and global
    variables'
      #if defined(USE_NEWMENU)
        defines
        global variable declarations
      #endif

  If the new menu requires that data be kept after the CameraAxe has been
    turned off, include in the section 'Memory offsets into EEPROM' an
    #ifdef reserving a place in EEPROM to store those values.
      #ifdef USE_NEWMENU
        EEPROM_NEWVALUE1,
        EEPROM_NEWVALUE2,
        EEPROM_ETC,
      #endif
  
  Your new menu will require a menu identifier which must be included in
    an #ifdef in the section labeled 'Start developer menu identifiers'.
    It should follow the format of the others   
      #ifdef USE_NEWMENU
        MENU_NEW,
      #endif
  
  If your new menu required eeprom locations, you will need to include
      in the section labeled 'Start developer eeprom default values'
      near the end of the loadDefaultSettings() function an #ifdef to
      set a reasonable default value for the new variable.
      #ifdef USE_NEWMENU
        eepromWriteInt(EEPROM_NEWVALUE1,    0);  //0 is the default value
        eepromWriteInt(EEPROM_NEWVALUE2,    1);  //1 is the default value
        eepromWriteInt(EEPROM_ETC,          9);  //9 is the default value
      #endif

  If your new menu required eeprom locations, you will also need to include
    in the section labeled 'Start developer eeprom validate values' an
      an #ifdef that checks the stored values to make sure they are within
      acceptable limits.
      #ifdef USE_NEWMENU
          eepromClamp(EEPROM_NEWVALUE1, 0, 9999); //low, high valid values
          eepromClamp(EEPROM_NEWVALUE2, 1,   50); //low, high valid values
          eepromClamp(EEPROM_ETC,      10, 2500); //low, high valid values
      #endif

  In the loop() function there will need to be two sets of #ifdef's to use
    new menu in menu mode and to execute the menu function in photo mode.
    In the section labeled 'Start developer menu modecalls' include an #ifdef
    to allow the menu to be accessed such as:
      #ifdef USE_NEWMENU        // change the ADD_NEW_MENU as appropriate
        case MENU_NEW:          // change MENU_NEW to the new case from above
          newMenu();            // change function name as appropriate
          break;                // donot forget to use the break statement
      #endif

    and in the section labeled 'Start developer photo mode calls' include
      an #ifdef to use the photo function associated with the new menu.

      #ifdef USE_NEWMENU        // change the ADD_NEW_MENU as appropriate
        case MENU_NEW:          // change MENU_NEW to the new case from above
          newFunc();            // change function name as appropriate
          break;                // donot forget to use the break statement
      #endif

  Of course you will need to write the menu code using the existing ones as
    examples and include them in the same directory as the CameraAxe.  The
    code should start with #ifdef USE_xxxMENU and end with #endif so that
    it can be conditionally compiled. You will undoubtedly need to create
    menu item strings. These should be placed in the language.ino file in
    the section labeled 'Start developer strings'. This will allow for future
    translation.

  You should include a Readme.txt with your code explaining what the new
    menu does and how it works, as well as some sample inputs and limitations
    (if any). This should allow easier inclusion in the User Manual.

To modify existing code....

  Include a #define USE_NEWCODE in the 'Start of User Selectable menus' so
    so that users can select to use/not use the new code. The characters
    'NEWCODE' should be replaced with your codes name.

  If the new menu depends on some other menu being selected... or is
    mutually exclusive of other menu items. Include #ifdefs or other
    preprocessor statements that require/prevent inclusion in the area
    just below the user selectable items
      #ifdef USE_NEWMENU
        #define USE_requiredMENU
      #endif
    where 'required' is replaced by the required menu name.

  If the new code requires another library, add the library just below the
    existing #includes

  If the new menu requires other #defines or new global variables include
    them in the section labeled 'Start developer #defines and global
    variables'
      #if defined(USE_NEWCODE)
        defines
        global variable declarations
      #endif

  If the new code requires that data be kept after the CameraAxe has been
    turned off, include in the section 'Memory offsets into EEPROM' an
    #ifdef reserving a place in EEPROM to store those values.
      #ifdef USE_NEWMENU
        EEPROM_NEWVALUE1,
        EEPROM_NEWVALUE2,
        EEPROM_ETC,
      #endif
  
  If your new code requires eeprom locations, you will need to include
      in the section labeled 'Start developer eeprom default values'
      near the end of the loadDefaultSettings() function an #ifdef to
      set a reasonable default value for the new variable.
      #ifdef USE_NEWCODE
        eepromWriteInt(EEPROM_NEWVALUE1,    0);  //0 is the default value
        eepromWriteInt(EEPROM_NEWVALUE2,    1);  //1 is the default value
        eepromWriteInt(EEPROM_ETC,          9);  //9 is the default value
      #endif

  If your new code requires eeprom locations, you will also need to include
    in the section labeled 'Start developer eeprom validate values' an
      an #ifdef that checks the stored values to make sure they are within
      acceptable limits.
      #ifdef USE_NEWCODE
          eepromClamp(EEPROM_NEWVALUE1, 0, 9999); //low, high valid values
          eepromClamp(EEPROM_NEWVALUE2, 1,   50); //low, high valid values
          eepromClamp(EEPROM_ETC,      10, 2500); //low, high valid values
      #endif

  Of course you will need to write the new code using the existing code as
    an example and include it in the appropriate file/s.  The code fragments
    should start with #ifdef USE_NEWCODE and end with #endif so that
    it can be conditionally compiled. You may need to create new strings.
    These should be placed in the language.ino file in
    the section labeled 'Start developer strings'. This will allow for future
    translation.

  You should include a Readme.txt with your code explaining what the new code
    does and how it works, as well as any limitations or potential problems
    (if any) that you know about. This should allow easier inclusion in the
    User Manual.
