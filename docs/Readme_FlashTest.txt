Readme_FlashTest.txt

INTRODUCTION
~~~~~~~~~~~~
This menu is for testing a flash Lag and flash duration.
It is also possible to measure your camera shutter-lag when you know your flash lag.

The flash lag is the time elapsed since the flash gets triggered until light is emitted.
Technically this is explained in Maurice's two TBP Videos:
- TPB #35 – Flash Lag (http://www.techphotoblog.com/tpb-35/)
- TPB #30 – Flash Duration and Intensity Comparison (http://www.techphotoblog.com/tpb-30/)

REQUIRED ITEMS
~~~~~~~~~~~~~~
For testing a Flash Device, the required items are:
- Camera Axe (obvious item)
- A Flashgun (to test its flash lag)
- Cable to connect CA to the device in test (Flashgun)
- Light Sensor. (I used the Light Sensor that can be purchased from Camera Axe store.
For testing your Camera sutter-lag, you also need:
- Cable to connect your camera to CA.

HARDWARE SETUP
~~~~~~~~~~~~~~
- Connect the camera or flash to "Camera/Flash1" port. 
- Connect the light sensor to "Sensor1 port.

MENU CONFIGURATION
~~~~~~~~~~~~~~~~~~
Two items are configurable here, which are:

- Recycle Time (in seconds): 
        This item is used to wait between two flashgun shots and also
        to allow the user to see and register the results from the last
        iteration. A value between 1sec up to 99 sec. (defaults to 5sec).
							 
- Loop Count:  
        Number of test iterations. If zero is provided, only one iteration is run.
        A value between zero to 99. (defaults to 10). The motivation is to allow the
        user to collect various values to see if consistent values are retrieved.

TEST ALGORITHM
~~~~~~~~~~~~~~
The following steps are done by this test after the user presses the Activate button:
 1) After initializing its variables, start with the first loop.
 2) Store the current time as startTime
 3) Trigger the device under test connected to Sensor1 port (It triggeres also Sensor2 Port)
 4) Wait for sensing an signal in the Light Sensor. 
    (If no light recorded after 1sec, signal for timeout)
 5) Once the Light Sensor triggered, record the time as riseTime.
 6) Wait for sensing that the signal in the Light Sensor went back to low.
   (If darkness recorded, signal for timeout)
 7) Once the light Sensor went back to low, record the time as fallTime
 8) If no Timeout Occurred, calculate:
    - Flash Lag(delay): riseTime - startTime,
	- Flash Duration: fallTime - riseTime,
	- Compute the Max/Min/Cumulative Lag/Duration time to provide statistics at the en
	  of the test.
 9) Display the Flash Running Menu while waiting for the Flash Recycle time, this is done in 
    order to provide the user with some feedback on the last recorded values.
10) If Loop Count is not zero, repeat the whole procedure (Steps 2 to 9) for the number 
    of times stored in Loop Count
11) If valid items were recorded (step 8 above), provide a summary menu with the results:
    - Mean (Average) Values
	- Max Values
	- Min Values.
12) Wait till the user presses the Activate button in order to return to the Setup menu.

In the case the user preses the "activate button" when the test is running, the code will
return to the setup menu at the end of the current loop.
              
LIMITATIONS:
~~~~~~~~~~~~
The following items are not "calibrated" or correctly retrieved:
- The Duration values seems to be too large, it seems that this is a problem with the way
  CA deals with a digital signal when an analog sensor is connected.
- The time that takes to signal the Flash in other menus is for FLASH_DURATION_MS (200ms).
  In this menu this value may be larger. (When connecting a Camera, this may issue more than
  a single shot if the shutter speed is less than ~ 1/5 sec.
  
Don't hesitate to ask in the Camera Axe Forum if you need help with this menu.

Have a nice day

--- Ricky Marek ( Ricky [dot] Marek [at] Gmail [dot] com
