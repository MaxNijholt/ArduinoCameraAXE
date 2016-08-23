////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maurice Ribble (Copyright 2010-2013)
// Camera Axe - http://www.cameraaxe.com
// Open Source, licensed under a Creative Commons Attribution-NonCommercial 3.0 License (http://creativecommons.org/licenses/by-nc/3.0/)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	irremote.pde - borrowed from Sebastian Setz project
			multicameraIRcontrol and modified slightly.
	
	g_shutterNow(device) is a global array that holds the function to be
	called if the camera device is to be controlled through IR signals.

	setShutterFunc() assigns the appropriate function pointer to the array. 	The correct function is called through setDevicePins().

	The actual code to send the IR codes are located in this file.
 */
#ifdef USE_IRREMOTEMENU
void wait(unsigned int time)
{
  unsigned long start = micros();

  while(micros()-start<=time)
  {
  }
}

void high(unsigned int time, int freq, int pinLED)
{
  int pause = (1000/freq/2)-4;
  unsigned long start = micros();

  while(micros()-start<=time)
  {
    digitalWrite(pinLED,HIGH);
    delayMicroseconds(pause);
    digitalWrite(pinLED,LOW);
    delayMicroseconds(pause);
  }
}

void setShutterFunc(int device, int camType)
{
	switch (camType)
	{
		case CANON_TYPE: 
		  //Serial.println("Canon Camera");
                  g_shutterNow[device] = CanonShutterNow;
                  break;
		case CANONWLDC100_TYPE: 
		  //Serial.println("CanonWLDC100 Camera");
                  g_shutterNow[device] = CanonWLDC100ShutterNow;
                  break;
		case MINOLTA_TYPE: 
		  //Serial.println("Minolta Camera");
                  g_shutterNow[device] = MinoltaShutterNow;
                  break;
		case NIKON_TYPE: 
		  //Serial.println("Nikon Camera");
                  g_shutterNow[device] = NikonShutterNow;
                  break;
		case OLYMPUS_TYPE: 
		  //Serial.println("Olympus Camera");
                  g_shutterNow[device] = OlympusShutterNow;
                  break;
		case PENTAX_TYPE: 
		  //Serial.println("Pentax Camera");
                  g_shutterNow[device] = PentaxShutterNow;
                  break;
		case SONY_TYPE: 
		  //Serial.println("Sony Camera");
                  g_shutterNow[device] = SonyShutterNow;
                  break;
                default:
		  //Serial.println("default: Nikon Camera");
                  g_shutterNow[device] = NikonShutterNow;
                  break;
	}
}

void sendFocusSignal(int device, int pin, int value)
{   //Not implemented
	if(value == LOW) return;
	return;
}

void sendShutterSignal(int device, int pin, int value)
{
	if(value == LOW) return;
	(*(g_shutterNow[device]))(pin);
}

void CanonShutterNow(int pin)
{
  for(int i=0; i<16; i++) 
  {
    digitalWrite(pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(pin, LOW);
    delayMicroseconds(11);
  }
  delayMicroseconds(7330);
  for(int i=0; i<16; i++) 
  {
    digitalWrite(pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(pin, LOW);
    delayMicroseconds(11);
  }
}

void CanonWLDC100ShutterNow(int pin) {
  int freq = 38;
  bool seq[]={0,1,0,1,0,0,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1};
  high(9042,freq,pin);
  wait(4379);
  for (unsigned int i=0;i<sizeof(seq);i++)
  {
    if (seq[i]==0)
    {
      high(612,freq,pin);
      wait(512);
    }
    else
    {
      high(612,freq,pin);
      wait(1621);
    }
  }
  high(599,freq,pin);
} 

void MinoltaShutterNow(int pin)
{
  int freq = 38;
  bool seq[]={0,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1};
  high(3750,freq,pin);
  wait(1890);
  for (unsigned int i=0;i<sizeof(seq);i++)
  {
    if (seq[i]==0)
    {
      high(456,freq,pin);
      wait(487);
    }
    else
    {
      high(456,freq,pin);
      wait(1430);
    }
  }
}

void NikonShutterNow(int pin)
{
  int freq = 40;
  //Serial.print("NikonShutterNow(pin=");
  //Serial.print(pin);
  //Serial.print(")\n");
  high(2000,freq,pin);
  wait(27830);
  high(390,freq,pin);
  wait(1580);
  high(410,freq,pin);
  wait(3580);
  high(400,freq,pin);
}

void OlympusShutterNow(int pin)
{
  int freq = 40;
  bool seq[]={0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1};
  high(8972,freq,pin);
  wait(4384);
  high(624,freq,pin);
  for (unsigned int i=0;i<sizeof(seq);i++)
  {
    if (seq[i]==0)
    {
      wait(488);
      high(600,freq,pin);
    }
    else
    {
      wait(1600);
      high(600,freq,pin);
    }
  }
}

void PentaxShutterNow(int pin)
{
  int freq = 38;
  high(13000,freq,pin);
  wait(3000);
  for (int i=0;i<7;i++)
  {
    high(1000,freq,pin);
    wait(1000);
  }
}

void SonyShutterNow(int pin)
{
  int freq = 40;
  bool seq[] = {1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1};
  for (int j=0;j<3;j++) 
  {
    high(2320,freq,pin);
    wait(650);
    for (unsigned int i=0;i<sizeof(seq);i++)
    {
      if (seq[i]==0)
      {
        high(575,freq,pin);
        wait(650);
      }
      else
      {
        high(1175,freq,pin);
        wait(650);
      }
    }
    wait(10000);
  }
}
#endif

