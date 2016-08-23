////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <avr/pgmspace.h>

// There are also a few string in cameraAxe.ino that need updating

PROGMEM const char _tx_99_version[]                 = "v5.6";
PROGMEM const char _tx_99_generalSettings[]         = "General Settings       ";
PROGMEM const char _tx_99_backlight[]               = "Backlight";
PROGMEM const char _tx_99_startupMode[]             = "Startup Mode";
PROGMEM const char _tx_99_presetGroup[]             = "Preset Group";
PROGMEM const char _tx_99_delayDecPlaces[]          = "Delay decimals";

PROGMEM const char _tx_99_backlightList[]           = "Off   |On    |10 sec";
PROGMEM const char _tx_99_startupList[]             = "Menu |Photo";
PROGMEM const char _tx_99_presetList[]              = "Preset_1|Preset_2|Preset_3|Preset_4";
PROGMEM const char _tx_99_delayDecPlaceList[]       = "1|3";

#ifdef USE_IRREMOTEMENU
PROGMEM const char _tx_99_camera1Type[]             = "Camera 1 Type";
PROGMEM const char _tx_99_camera2Type[]             = "Camera 2 Type";
PROGMEM const char _tx_99_deviceTypeList[]          = "Wired   |Canon   |Canon100|Minolta |Nikon   |Olympus |Pentax  |Sony    ";
#endif

#ifdef USE_ADVANCEDMENU
PROGMEM const char _tx_00_advancedSensorMenu[]      = "Advanced Sensor Menu";
PROGMEM const char _tx_00_device1Device2[]          = "         Device1  Device2";
PROGMEM const char _tx_00_trigSen[]                 = "Trig Sen";
PROGMEM const char _tx_00_delayms[]                 = "Delay ms";
PROGMEM const char _tx_00_bulbSec[]                 = "Bulb Sec";
PROGMEM const char _tx_00_prefocus[]                = "Prefocus";
PROGMEM const char _tx_00_sensor1Sensor2[]          = "         Sensor1  Sensor2";
PROGMEM const char _tx_00_trigType[]                = "Trig Typ";
PROGMEM const char _tx_00_trigVal[]                 = "Trig Val";
PROGMEM const char _tx_00_power[]                   = "Power";
PROGMEM const char _tx_00_advancedPhotoMode[]       = "Advanced Photo Mode";
PROGMEM const char _tx_00_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const char _tx_00_hi[]                      = "Hi ";
PROGMEM const char _tx_00_low[]                     = "Low";
PROGMEM const char _tx_00_noYesList[]               = "No |Yes";
PROGMEM const char _tx_00_sensorList[]              = "None    |Sensor1 |Sensor2 |S1_or_S2|S1_and_2";
PROGMEM const char _tx_00_trigTypeList[]            = "Low     |High    |Threshld|Digital";
PROGMEM const char _tx_00_lowHighList[]             = "Low|Hi ";
PROGMEM const char _tx_00_powerList[]               = "On      |Off_Dev1|Off_Dev2";
PROGMEM const char _tx_00_camFlashList[]            = "Normal   |Pre-focus";
#endif

#ifdef USE_PROJECTILEMENU
PROGMEM const char _tx_01_projectileMenu[]          = "Projectile Menu";
PROGMEM const char _tx_01_distance[]                = "Distance";
PROGMEM const char _tx_01_lowHighTrigger[]          = "Low/High Trigger";
PROGMEM const char _tx_01_distanceUnits[]           = "Distance Units";
PROGMEM const char _tx_01_projectilePhotoMode[]     = "Projectile Photo Mode";
PROGMEM const char _tx_01_sensor2DidNotTrigger[]    = " Sensor2 did not trigger ";
PROGMEM const char _tx_01_readyForProjectile[]      = "  Ready for projectile!  ";
PROGMEM const char _tx_01_feet[]                    = "Feet";
PROGMEM const char _tx_01_inches[]                  = "Inches";
PROGMEM const char _tx_01_meters[]                  = "Meters";
PROGMEM const char _tx_01_centimeters[]             = "Centimeters";
PROGMEM const char _tx_01_sec[]                     = "/sec: ";
PROGMEM const char _tx_01_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const char _tx_01_lowHighList[]             = "Low |High";
PROGMEM const char _tx_01_inchCmList[]              = "Inch|Cm  ";
#endif

#ifdef USE_VALVEMENU
PROGMEM const char _tx_02_valveMenu[]             = "Valve Menu";
PROGMEM const char _tx_02_calibrate[]             = "Auto Calibrate";
PROGMEM const char _tx_02_numValves[]             = "Num Valves";
PROGMEM const char _tx_02_flashDelayBase[]        = "Flash Based on Start";
PROGMEM const char _tx_02_Drop1Delay[]            = " Start Offset";
PROGMEM const char _tx_02_Drop1Size[]             = " Drop1 Size";
PROGMEM const char _tx_02_Drop2Delay[]            = " Drop2 Delay";
PROGMEM const char _tx_02_Drop2Size[]             = " Drop2 Size";
PROGMEM const char _tx_02_Drop3Delay[]            = " Drop3 Delay";
PROGMEM const char _tx_02_Drop3Size[]             = " Drop3 Size";
PROGMEM const char _tx_02_flashDelay[]            = "Flash Delay";
PROGMEM const char _tx_02_flashDelayAutoInc[]     = "Flash Delay Auto Inc";
PROGMEM const char _tx_02_numShots[]              = "Number of Shots";
PROGMEM const char _tx_02_secBetwnShots[]         = "Sec Between Shots";
PROGMEM const char _tx_02_repeatShots[]           = "Number of Repeats";
PROGMEM const char _tx_02_flash1[]                = "Use Flash1 Port As";
PROGMEM const char _tx_02_valvePhotoMode[]        = "Valve Photo Mode";
PROGMEM const char _tx_02_valveActive[]           = "       Valve Active";
PROGMEM const char _tx_02_camFlashList[]          = "Cam|Fla";
PROGMEM const char _tx_02_noYesList[]             = "No |Yes";
PROGMEM const char _tx_02_calibrationMenu[]       = "Auto Calibration Menu";
PROGMEM const char _tx_02_calibrationText1[]      = "First wait for tallest";
PROGMEM const char _tx_02_calibrationText2[]      = "               splash.";
PROGMEM const char _tx_02_calibrationText3[]      = "Then wait for nice drop";
PROGMEM const char _tx_02_calibrationText4[]      = "             collision.";
PROGMEM const char _tx_02_calibrationText5[]      = "      Press Select";
#endif

#ifdef USE_INTERVALOMETERMENU
PROGMEM const char _tx_03_intervalometerMenu[]      = "Intervalometer Menu";
PROGMEM const char _tx_03_startDelay[]              = "Start Delay";
PROGMEM const char _tx_03_interval[]                = "Interval";
PROGMEM const char _tx_03_shots[]                   = "# Shots (0=Inf)";
PROGMEM const char _tx_03_bulb[]                    = "Bulb (sec)";
PROGMEM const char _tx_03_hdrStops[]                = "HDR Stops";
PROGMEM const char _tx_03_mirrorLockup[]            = "Mirror Lockup";
PROGMEM const char _tx_03_timeColon[]               = "  :  :";
PROGMEM const char _tx_03_intervalometerPhotoMode[] = "Intervalometer Photo Mode";
PROGMEM const char _tx_03_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const char _tx_03_noYesList[]               = "No |Yes";
#endif

#ifdef USE_GRAVITYMENU
PROGMEM const char _tx_04_gravityMenu[]             = "Gravity Menu";
PROGMEM const char _tx_04_distance[]                = "Distance";
PROGMEM const char _tx_04_distanceUnits[]           = "Distance Units";
PROGMEM const char _tx_04_shutterLag[]              = "Shutter Lag (ms)";
PROGMEM const char _tx_04_gravityPhotoMode[]        = "Gravity Photo Mode";
PROGMEM const char _tx_04_sensor2DidNotTrigger[]    = " Sensor2 did not trigger ";
PROGMEM const char _tx_04_readyForObject[]          = "  Ready for object!  ";
PROGMEM const char _tx_04_startingHeight[]          = "Starting Height: ";
PROGMEM const char _tx_04_inch[]                    = " inch";
PROGMEM const char _tx_04_cm[]                      = " cm";
PROGMEM const char _tx_04_sensorTime[]              = "Sensor Time: ";
PROGMEM const char _tx_04_ms[]                      = " ms";
PROGMEM const char _tx_04_fallTime[]                = "Fall Time: ";
PROGMEM const char _tx_04_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const char _tx_04_inchCmList[]              = "Inch|Cm  ";
#endif

#ifdef USE_STACKERMENU
PROGMEM const char _tx_05_stackerMenu[]             = "Stacker Menu";
PROGMEM const char _tx_05_motorLoc[]                = "Motor Location";
PROGMEM const char _tx_05_picsStack[]               = "Photos/Stack";
PROGMEM const char _tx_05_motorDir[]                = "Motor Direction";
PROGMEM const char _tx_05_distUnit[]                = "Distance Unit";
PROGMEM const char _tx_05_stepsUnit[]               = "Steps/Unit";
PROGMEM const char _tx_05_totalDist[]               = "Total Distance";
PROGMEM const char _tx_05_settle[]                  = "Settle Time(ms)";
PROGMEM const char _tx_05_speedStepsSec[]           = "Speed(Steps/Sec)";
PROGMEM const char _tx_05_bulbSec[]                 = "Bulb Timer (sec)";
PROGMEM const char _tx_05_stackerPhotoMode[]        = "Stacker Photo Mode";
PROGMEM const char _tx_05_photosTaken[]             = "Photos Taken";
PROGMEM const char _tx_05_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const char _tx_05_photoStackComplete[]      = "  Photo Stack Complete!!";
PROGMEM const char _tx_05_locationList[]            = "SENSOR 1|SENSOR 2";
PROGMEM const char _tx_05_unitsList[]               = "INCHES|CM    ";
PROGMEM const char _tx_05_dirList[]                 = "FORWARD|REVERSE";
#endif

#ifdef USE_PANORAMAMENU
PROGMEM const char _tx_06_panoramaMenu[]            = "Panorama Menu";
PROGMEM const char _tx_06_motorLoc[]                = "Motor Location";
PROGMEM const char _tx_06_picsPano[]                = "Photos/Pano";
PROGMEM const char _tx_06_motorDir[]                = "Motor Direction";
PROGMEM const char _tx_06_stepsRevolution[]         = "Steps/Revolution";
PROGMEM const char _tx_06_totDegrees[]              = "Total Degrees";
PROGMEM const char _tx_06_settle[]                  = "Settle Time(ms)";
PROGMEM const char _tx_06_speedStepsSec[]           = "Speed(Steps/Sec)";
PROGMEM const char _tx_06_bulbSec[]                 = "Bulb Timer (sec)";
PROGMEM const char _tx_06_panoramaPhotoMode[]       = "Panorama Photo Mode";
PROGMEM const char _tx_06_photosTaken[]             = "Photos Taken";
PROGMEM const char _tx_06_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const char _tx_06_panoramaComplete[]        = "   Panorama Complete!!!";
PROGMEM const char _tx_06_locationList[]            = {"SENSOR 1|SENSOR 2"};
PROGMEM const char _tx_06_dirList[]                 = {"FORWARD|REVERSE"};
#endif

#ifdef USE_JOGMENU
PROGMEM const char _tx_07_jogMotorMenu[]            = "Jog Motor Menu";
PROGMEM const char _tx_07_motorLocation[]           = "Motor Location";
PROGMEM const char _tx_07_stepsJogLR[]              = "Steps/Jog(L/R)";
PROGMEM const char _tx_07_stepsJogUD[]              = "Steps/Jog(U/D)";
PROGMEM const char _tx_07_speed[]                   = "Speed(Steps/Sec)";
PROGMEM const char _tx_07_jogMotorMode[]            = "Jog Motor Mode";
PROGMEM const char _tx_07_direction[]               = "Direction";
PROGMEM const char _tx_07_jogMode[]                 = "Jog Mode";
PROGMEM const char _tx_07_stepsJog[]                = "Steps/Jog";
PROGMEM const char _tx_07_totSteps[]                = "Tot Steps";
PROGMEM const char _tx_07_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const char _tx_07_forward[]                 = "Forward ";
PROGMEM const char _tx_07_backward[]                = "Backward";
PROGMEM const char _tx_07_single[]                  = "Single    ";
PROGMEM const char _tx_07_continuous[]              = "Continuous";
PROGMEM const char _tx_07_locationList[]            = "SENSOR 1|SENSOR 2";
#endif

#ifdef USE_FIREWORKSMENU
PROGMEM const char _tx_08_fireworksMenu[]           = "Fireworks Menu";
PROGMEM const char _tx_08_bulb[]                    = "Bulb Duration"; 
PROGMEM const char _tx_08_trigVal[]                 = "Trigger Value";
PROGMEM const char _tx_08_numTrig[]                 = "Num Triggers";
PROGMEM const char _tx_08_fireworksMode[]           = "Fireworks Mode";
PROGMEM const char _tx_08_pressActivateToExit[]     = "  Press Activate to Exit";
#endif

#ifdef USE_FLASH_TEST_MENU
PROGMEM const char _tx_09_flashTestMenu[]           = "Flash Test Menu";
PROGMEM const char _tx_09_flashTestPhotoMode[]      = "Flash Test Running";
PROGMEM const char _tx_09_flashTestResultsMode[]    = "Flash Test Results";
PROGMEM const char _tx_09_pressActivateToExit[]     = "Press Activate to Exit";
PROGMEM const char _tx_09_flashRecycleTime[]        = "Recycle Time[sec]";
PROGMEM const char _tx_09_flashLoopCount[]          = "Loop Count";
PROGMEM const char _tx_09_flashLagTime[]            = "Lag[us]";
PROGMEM const char _tx_09_flashDurationTime[]       = "Duration[us]";
PROGMEM const char _tx_09_meanValue[]               = "Mean";
PROGMEM const char _tx_09_maxValue[]                = "Max.";
PROGMEM const char _tx_09_minValue[]                = "Min.";
PROGMEM const char _tx_09_validItems[]              = "Valid Samples";
PROGMEM const char _tx_09_dummy[]                   = " ";
#endif

#ifdef USE_ROLLERCOASTERMENU
PROGMEM const char _tx_10_rolllercoasterMenu[]      = "Roller Coaster Menu";
PROGMEM const char _tx_10_trigVal[]                 = "Trig Value";
PROGMEM const char _tx_10_trigType[]                = "Trig Type";
PROGMEM const char _tx_10_resetTime[]               = "Reset Time sec";
PROGMEM const char _tx_10_preFocus[]                = "Pre-Focus";
PROGMEM const char _tx_10_numShots[]                = "# of Shots";
PROGMEM const char _tx_10_timeBetweenShots[]        = "Shot Time ms";
PROGMEM const char _tx_10_prefocusList[]            = "Normal   |Pre-focus";
PROGMEM const char _tx_10_lowHighList[]             = "Low|Hi ";
PROGMEM const char _tx_10_pressActivateToExit[]     = "  Press Activate to Exit";
#endif

#ifdef USE_DONGLEMENU
PROGMEM const char _tx_11_dongleMenu[]              = "Dongle Menu";
PROGMEM const char _tx_11_triggertrap[]             = "visit www.triggertrap.com";
PROGMEM const char _tx_11_pressActivateToExit[]     = "  Press Activate to Exit";
#endif

#ifdef USE_MAXSONARMENU
PROGMEM const char _tx_12_maxsonarMenu[]            = "MaxSonar Menu";
PROGMEM const char _tx_12_trigWidth[]               = "Trigger Width(cm)";
PROGMEM const char _tx_12_cam1Delay[]               = "Cam1 Delay(ms)";
PROGMEM const char _tx_12_cam1LowLat[]              = "Cam1 Low Latency";
PROGMEM const char _tx_12_cam1Duration[]            = "Cam1 Duration(ms)";
PROGMEM const char _tx_12_cam2Delay[]               = "Cam2 Delay(ms)";
PROGMEM const char _tx_12_cam2LowLat[]              = "Cam2 Low Latency";
PROGMEM const char _tx_12_cam2Duration[]            = "Cam2 Duration(ms)";
PROGMEM const char _tx_12_timeBetweenShots[]        = "Time Btwn Shots(ms)";
PROGMEM const char _tx_12_noYesList[]               = "No |Yes";
PROGMEM const char _tx_12_pressActivateToExit[]     = "  Press Activate to Exit";
#endif

#ifdef USE_LIGHTNINGMENU
PROGMEM const char _tx_13_lightningMenu[]           = "Lightning Menu V2.1";
PROGMEM const char _tx_13_trigVal[]                 = "Trig Val";
PROGMEM const char _tx_13_nullnull[]                = " ";
PROGMEM const char _tx_13_UpdtMS[]                 = "Updt(ms)";
PROGMEM const char _tx_13_DisplayMode[]		            = "Display";
PROGMEM const char _tx_13_DisplayModeList[]	          = "Simple  |Advanced";
PROGMEM const char _tx_13_Camera1Camera2[]          = "         Camera1  Camera2";
PROGMEM const char _tx_13_prefocus[]                = "Prefocus";
PROGMEM const char _tx_13_bulbSec[]                 = "Bulb Sec";
PROGMEM const char _tx_13_lightningPhotoModeSimple[]= "Lightning Photo Mode V2.1";
PROGMEM const char _tx_13_lightningPhotoModeAdvanced[] = "Ltg Adv-Activate to Exit";
PROGMEM const char _tx_13_lightningPhotoModeAdvTitles[] = "Stk# Ref   Pk  Duratn(Us)";
PROGMEM const char _tx_13_pressActivateToExit[]     = "  Press Activate to Exit";
PROGMEM const char _tx_13_noYesList[]               = "No |Yes";
PROGMEM const char _tx_13_camFlashList[]            = "Normal   |Pre-focus";
PROGMEM const char _tx_13_ReferenceBase[]           = "Reference Base";
PROGMEM const char _tx_13_CurrentValDif[]           = "Current Val/Dif";
PROGMEM const char _tx_13_ThresholdTriggerValue[]   = "Trigger Value";
PROGMEM const char _tx_13_UpdateRefMS[]            = "Update Ref (ms)";
PROGMEM const char _tx_13_TrigCntPeakVal[]         = "TrigCnt/PeakVal";

#endif


/////////// Start developer strings ///////////////////////////////////////////

///////////// End developer strings ///////////////////////////////////////////

char *getStr(const char* inStr)
{
  byte i = -1;

  // Search for null terminated string or use the special '|' char to terminate a string
  // The '|' termination makes it easier to do lists of strings in PROGMEM
  do {
    ++i;
    g_cbuf[i] = pgm_read_byte_near(inStr+i);
  }while (g_cbuf[i] != '\0' && g_cbuf[i] != '|');
 
  g_cbuf[i] = '\0';
  return g_cbuf;
}
