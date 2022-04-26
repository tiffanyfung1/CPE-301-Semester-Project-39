// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>

// Main program constants and variables
#define LCD_REFRESH 60
#define TEMPERATURE_LIMIT ##
int state; // 0=disabled, 1=idle, 2=running, 3=error

// I/O Macros
#define LEDS_OFF()     *ledPort &= ~(0x##);
#define LED_ON(pinNum) *ledPort |= (0x01 << pinNum);

void setup(){  
  Serial.begin(9600);
  setState(0); // disabled
  resetTimer(LCD_REFRESH);
}

void loop(){
  // If state != 0 & fan adjust input
    // Adjust fan position
    // Serial port: report vent position change
  
  /*if(state = 1 & getTemperature() > TEMPERATURE_LIMIT){
    setFan(1);   // fan on
    setState(2); // running
  }*/

  /*if(state = 2 & getTemperature() < TEMPERATURE_LIMIT){
    setFan(0);   // fan off
    setState(1); // idle
  }*/
}

// Change cooler state
void setState(int newState){
  state = newState;
  // Serial port: report time and newState
  LEDS_OFF();
  LED_ON(newState);
}

// Resets LCD refresh timer to 1 minute
void resetTimer(int seconds){
  
}

// Reads the current air temperature
int getTemperature(){
  
}

// Turns fan motor on and off
void setFan(int fanOn){
  // fanOn = 0 -> off, fanON = 1 -> on
}

/* Start Button Interrupt:
  // Turn on LCD
  // setState(1); // idle
*/

/* Stop Button Interrupt:
  // Turn off LCD
  // setFan(0);   // fan off
  // setState(0); // disabled
*/

/* Reset Button Interrupt:
  // If waterLevel > threshold
    // setState(1); // idle
*/

/* Timer Interrupt:
  if(state != 0){
    // Display humidity and temperature on LCD
  }
  // resetTimer(LCD_REFRESH);
*/

// NOTE: Can also be done using ADC
/* Comparator Interrupt: // waterLevel < threshold
  // Write error message to LCD
  // setFan(0);   // fan off
  // setState(3); // error
*/
