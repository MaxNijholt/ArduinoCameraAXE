IR Camera Control with CameraAxe

In order to use IR control for your camera through the CameraAxe, you will need and IR emitter(IR LED) device to plug into one of the camera ports. Previously the IR emitter device I created was smart. It included not only the IR emitter but also a microprocessor (Ardweeny from Solarbotics.com) and a set of switches. You selected the camera type with the switches and the microprocessor would cause the IR emitter to send the appropriate code. The advantage of this is that nothing at all needs to change in the CameraAxe. The CameraAxe will not know if you are using IR control or not.

In order to try to simplify the IR emitter, it was suggested that the CameraAxe directly control IR LED. This would reduce the cost of the IR emitter device and allow easy expansion of the various codes and functions to be sent to the camera.  The downside is that the user must now select the camera type and indicate if it is using IR control or not and more code space would be consumed in the CameraAxe.

For this simplified IR emitter, I have tried to create a menu system and hardware to allow for easy incorporation into the CameraAxe. This called for a couple of global variables, a couple of menu items in general settings, a few lines of code in the setDevicePins() function and an additional file of IR codes for some popular cameras.

Hardware:
The IR emitter is simplicity itself. I have tried both of these configurations with success. From the CameraAxe side, it does not make any difference which emitter configuration you choose. In my limited tests the range ends up being less than 10ft. This may be due to the IR LED choosen. The official Nikon IR remote and its knockoffs seem to work at 40ft or more.

/////////////////////////////////////////////////////////////////
This is a simple circuit that will do about 50ma which is the max of the opto
isolator in the CameraAxe.
	(Vbat - LED Vf - Opto Vsat) / 50mA = R
	(  9  -   1.2  -    .6    ) / .05  = 144 ohm

Use 150ohm as it is the next higher standard resistor value.
This works at 5 to 8ft reliably for my Nikon D5000. 10ft does not work.



  +---/ --------/\/\/-----|>|-----+---------+ tip (signal)
  |  SW1        150ohm    IR LED
  +
 9V Bat                                     + mid (nc)
  -
  |
  +-----------------------------------------+ ring (gnd)


/////////////////////////////////////////////////////////////////
If you need more power, this should do the trick. We need a PNP
transistor that will carry 100mA - Radio Shack 2N3906 #276-1604


                E     C
                  PNP           Anode  Cathode
        +---/ --->   +----/\/\/------|>|---+
    POS |  SW1    \_/     82ohm     IRLED  |
        +        B |                       |
    Bat 9V         |                       |
        -          |                       |
    NEG |          |                       |
        +----------|-----------------------+-------+ ring (gnd)
                   |            
                   |                               + mid (nc)
                   |  10K       
                   +-/\/\/-------------------------+ tip (signal)
                     pullup

IR_LED:	Arduino pins configured as output can source or sink 40mA.
	The CameraAxe opto-isolator is limited to 50 mA.
	This is not enough current to drive this LED to maximum output.
	We must use a transistor as a switch to allow the LED to draw the
	maximum allowable current.

	RadioShack			276-0143
	Size				T-13/4
	Radiant Power@100ma		16mW min
	Forward Volts(Vf)		1.2V
	Forward Current(Vc)		100mA
	Wavelength			940nm
	Viewing Angle to 1/2 intensity	45 deg

Software:
	CameraAxe.pde:
	Just after #include <DogmCA.h> add in the global variables

enum {CANON_TYPE, CANONWLDC100_TYPE, MINOLTA_TYPE, NIKON_TYPE, OLYMPUS_TYPE, PENTAX_TYPE, SONY_TYPE}; // camera types
enum {WIRED, IRREMOTE, RADIOREMOTE};	// control types (RADIOREMOTE not used)
void setShutterFunc(int device, int deviceType); //assign appropriate codes
int g_controlType[2];              //wired or ir_remote for device 1 and 2
void (*g_shutterNow[2])(int pin);  //actual function to call for shutter release

	A couple of new EEPROM memory locations need to be defined (I put mine directly after EEPROM_GENERAL_STARTUP_MODE). Just number consecutively remembering to renumber the EEPROM_GENERAL PRESET_GROUP and EEPROM_SIZE.

#define EEPROM_CONTROL1_TYPE                  45
#define EEPROM_DEVICE1_TYPE                   46
#define EEPROM_CONTROL2_TYPE                  47
#define EEPROM_DEVICE2_TYPE                   48
#define EEPROM_GENERAL_PRESET_GROUP           49
#define EEPROM_SIZE                           50

	In setup(), the values need to be read so they can be used at startup,so at the end of setup() just after g_menuMode = eepromReadInt(EEPROM_GENERAL_STARTUP_MODE); add the 4 lines.

  g_controlType[0] = eepromReadInt(EEPROM_CONTROL1_TYPE);
  g_controlType[1] = eepromReadInt(EEPROM_CONTROL2_TYPE);
  setShutterFunc(0, eepromReadInt(EEPROM_DEVICE1_TYPE));
  setShutterFunc(1, eepromReadInt(EEPROM_DEVICE2_TYPE));

	These new memory locations need default values so in loadDefaultSettings() add a couple of lines just after eepromWriteInt(EEPROM_GENERAL_STARTUP_MODE, 0);

    eepromWriteInt(EEPROM_CONTROL1_TYPE,       WIRED);
    eepromWriteInt(EEPROM_DEVICE1_TYPE,        NIKON_TYPE);  //or whatever enum you prefer
    eepromWriteInt(EEPROM_CONTROL2_TYPE,       WIRED);
    eepromWriteInt(EEPROM_DEVICE2_TYPE,        NIKON_TYPE);  //or whatever enum you prefer

    	Values stored also need to be validated so at the end of validateEepromValues() add the lines.

    eepromClamp(EEPROM_CONTROL1_TYPE,     WIRED, RADIOREMOTE);		//first and last
    eepromClamp(EEPROM_DEVICE1_TYPE,      CANON_TYPE, SONY_TYPE);	//first and last
    eepromClamp(EEPROM_CONTROL2_TYPE,     WIRED, RADIOREMOTE);		//first and last
    eepromClamp(EEPROM_DEVICE2_TYPE,      CANON_TYPE, SONY_TYPE);	//first and last

	menu99_settings.pde:
	This is where the user selects whether they want to use either WIRED control (normal) or IRREMOTE control. If you select IRREMOTE you must also select the camera type so the CameraAxe knows what codes to send. Menu navigation is hard to explain so I will just refer you to the code.


	cameraAxe_helperFuncs.pde:
	The camera is triggered by the function setDevicePins(). This function needs to be modified to send out the appropriate pulses when called.

void setDevicePins(byte device, byte focus, byte shutter)
{
  if(g_controlType[device])	//if this device is IRREMOTE
  {
    sendFocusSignal(device, DEVICE1_FOCUS_PIN+device*2, focus); //not used
    sendShutterSignal(device, DEVICE1_SHUTTER_PIN+device*2, shutter);
  }
  else
  {
    digitalWrite(DEVICE1_FOCUS_PIN+device*2, focus);
    digitalWrite(DEVICE1_SHUTTER_PIN+device*2, shutter);
  }
}

	Last but not least, a file with the IR codes and functions must be included. Just place the file in the CameraAxe folder. This code is borrowed from Sebastian Setz's project. It could be improved but leaving it mostly as is means that it has been tested by many others as part of Sebastian's project.
