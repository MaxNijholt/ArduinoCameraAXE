Readme_Motor.txt

The motor function is part of the CameraAxe as an inactive beta test. This shows that the motor sensor basic functions work. I have added two functions but I cannot do any realworld testing as I don't actually have the motor attached to anything.
It does seem like a worthwhile addition to the motor sensor would be the addition of limit switches that could turn off power to the motor if the motor movement goes to far.
From my perspective, the hard part is coming up with easy to use menus that will fit a majority of situations. I have added 3 menus: stacker, panorama and jog.
The stacker menu allows the user to specify a distance to travel and the number of photos to
take. The CA will figure out how far to move between each picture.
The actual menu items are:
	Motor Location(which sensor plug)
	Pics/Stack (number of photos to take)
	Motor Dir(Forward or Backward)
	Dist Unit (inches, cm)
	Steps/Unit
	Total Dist
	Settle(ms) (settle time after movement)
	Speed(Steps/Sec)

The panorama menu is similar but the distance is expressed in degrees of rotation.
	Motor Location(which sensor plug)
	Pics/Pano (number of photos to take)
	Motor Dir(Forward or Backward)
	Steps/Revolution
	Tot Degrees
	Settle(ms) (settle time after movement)
	Speed(Steps/Sec)

The jog menu allows you to position the motor
	Motor Location(which sensor plug)
	Steps/Jog(L/R) (number of steps to take for each button push left or right)
	Steps/Jog(U/D) (number of steps to take for each button push up or down)
	Speed(Steps/Sec)

	Activate puts you in the jog execution menu. Once in this menu Select will change the
	operation from a single move for a button press to continuous movement while holding the
	L/R/U/D buttons.
