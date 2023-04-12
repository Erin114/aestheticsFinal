//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 ***** Please set the number of devices you have *****
 But the maximum default of 8 MAX72XX wil also work.
 */
LedControl lc=LedControl(8,6,7,8);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;

/* 
 This time we have more than one device. 
 But all of them have to be initialized 
 individually.
 */
void setup() {
  //we have already set the number of devices when we created the LedControl
  int devices=lc.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,8);
    /* and clear the display */
    lc.clearDisplay(address);
  }
}

void loop() { 
  //read the number cascaded devices
  int devices=lc.getDeviceCount();
  
  /*
    In order to get the addresses to index properly, you must invert the address call for
    half of the matrices. Address 7 is parallel to 1 with inverted row/col coordinates, 2 is
    parallel to 6, so on and so forth. (0,0) on addresses 0-3 is (7,7) on 4-7
  */
  //we have to init all devices in a loop

  for (int y = 0; y < 32; y++)
  {
    ledCoordinate(15,y,true);
    ledCoordinate(0,y,true);
    delay(delaytime);
    ledCoordinate(15,y,false);
    ledCoordinate(0,y,false);


  }
        
}

void ledCoordinate(int x, int y, bool status)
{
  if (x > 15 || y > 31) {return;}
  bool invert = false;
  int address = 0;
  if (x > 7)
  {
    invert = true;    
  }
  if (y >= 8)
  {address = 1;}
  if (y >= 16) 
  {address = 2;}
  if (y >= 24)
  {address = 3;}

  if (invert == true)
  {
    lc.setLed(7-address,7-(x%8),(y%8),status);
  }
  else
  {
     lc.setLed(address,(x%8),7-(y%8),status);
  }
  

}
