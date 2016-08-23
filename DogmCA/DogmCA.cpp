/*
  Modified for use with Camera Axe with written permission from Oliver Kraus.
  Maurice Ribble - 9/4/2010
  
  Now shared under Creative Commons Attribution 3.0 License
    (http://creativecommons.org/licenses/by-sa/3.0/)
*/   

#include <avr/io.h>
#include <DogmCA.h>

void DogmCA::Init(void)
{
  is_req_init = 0;
  dog_init(a0Pin);
}

DogmCA::DogmCA(uint8_t pin_a0)
{
  size = 0;
  tx = 0;
  ty = 0;
  a0Pin = pin_a0;
  is_req_init = 1;
  // why can dog_init() not be called here... arduino will hang if this is done in the constructor
  // should be investigated some day
  // dog_init(pin_a0);
  
}

void DogmCA::start(void)
{
  if ( is_req_init )
    Init();
  dog_StartPage();
}


