//We always have to include the library
#include "LedControl.h"

enum pointState {
  off,
  on,
  dead
};

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
pointState matrix[32][16] = {};


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

  for (int x = 0; x < 16; x++)
  {
    ledCoordinate(x,0,true);
    ledCoordinate(15-x,31,true);
    delay(delaytime);
    ledCoordinate(x,0,false);
    ledCoordinate(15-x,31,false);


  }
        
}

//converts cartesian coordinates into the coordinates used by the LED matrix
// x range 0-15, y range 0-31, true = on, false = off
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


bool is_neighbor_on(int x, int y, int width, int height, pointState ** grid) 
{
  return ( ( x > 0 && grid[x-1][y] == on )
    || ( y > 0 && grid[x][y-1] == on )
    || ( x < width - 1 && grid[x+1][y] == on )
    || ( y < height - 1 && grid[x][y+1] == on ) );
}

pointState step_point(int x, int y, int width, int height, pointState ** grid)
{
  pointState current_state = grid[x][y];
  if (current_state == on) 
  {
    return dead;
  }
  else if (current_state == dead)
  {
    return off;
  }
  else 
  {
    if (is_neighbor_on(x, y, width, height, grid))
    {
      return on;
    }
    else 
    {
      return off;
    }
  }
}

pointState ** step_grid(int x, int y, int width, int height, pointState ** grid, pointState ** out)
{
  for (int row = 0; row < height; row++) 
  {
    for (int column = 0; column < width; column++) 
    {
      out[row][column] = step_point(row, column, width, height, grid);
    }
  }

  return out;
}