// CPE 301 Semester Project
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

void setup() {
  // put your setup code here, to run once:

}

void loop() {// CPE 301 Semester Project
// Group 39
// Parker True, Tiffany Fung, Bryce Millis
#include <Arduino.h>

int min_threshold = 310;
int max_threshold = 510;

//need storing water value, should start at 0

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // start with disabled() 
  // then if statements within this?
  //if disabled is turn on, call idle()

}

// LCD needs to be connected to idle() and running() 
idle()
{
  //green LED
  // if temperature is greater than the threshold, call running()
  // if water is <= to thres, call error()
  // if stop button is pressed, call disabled()
}


running()
{
  // blue LED
  // if temperature is <= thres, call idle()
  // if water < thres, call error()
  // if stop button is pressed, call disabled()
}

error()
{
  // if reset button is pressed, call idle()
  // if stop button is pressed, call disabled()
 
}

disabled()
{
  // if start button is pressed, call idle()
}




  // put your main code here, to run repeatedly:

}
