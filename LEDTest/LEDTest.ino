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
  int addInv = 7;
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      addInv = 7;
      for(int address=0;address<devices/2;address++) {
        
         // delay(delaytime);
        lc.setLed(address,row,col,true);
        lc.setLed(addInv,7-row,7-col,true);
        delay(delaytime);
        lc.setLed(address,row,col,false);
        lc.setLed(addInv,7-row,7-col,false);
        addInv--;
        
      }
    }
  }
}
