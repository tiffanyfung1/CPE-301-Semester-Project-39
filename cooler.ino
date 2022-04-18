// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>

// Serial port state
#define RDA 0x80
#define TBE 0x20  
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;

// Water level thresolds
#define MIN_THRESHOLD 310
#define MAX_THRESHOLD 510

// Main program variables
int state; // 0=disabled, 1=idle, 2=running, 3=error
int waterLevel;

void setup(){  
  state = 0; // disabled
  U0init(9600); // initialize the serial port on USART0:
}

void loop(){
  // Switch statement selects function for current state
  // Current state function returns next state
  switch(state){
    case 0:
      state = disabled(); 
      break;
    case 1:
      state = idle();
      break;
    case 2:
      state = running();
      break;
    case 3: 
      state = error();
    };
}

int disabled(){
  // turn off LCD
  // turn on yellow LED
  // start button pressed -> idle
}

void disabledCleanup(){
  // turn on LCD
  // turn off yellow LED
  // report state transition on Serial port
}

int idle(){
  // turn on green LED
  // temperature > threshold -> running
  // water <= threshold -> error
  // stop button pressed -> disabled
}

void idleCleanup(){
  // turn off green LED
  // report state transition on Serial port
}

int running()
{
  // start fan motor
  // turn on blue LED
  // temperature <= threshold -> idle
  // water < thres -> error
  // stop button pressed -> disabled
}

void runningCleanup(){
  // stop fan motor
  // turn off blue LED
  // report state transition on Serial port
}

int error()
{
  // turn on red LED
  // write error message to LCD
  // reset button is pressed & water > threshold -> idle
  // stop button is pressed -> disabled
}

void errorCleanup(){
  // turn off red LED
  // report state transition on Serial port
}

// Function to initialize USART0 to "int" Baud, 8 data bits,
// no parity, and one stop bit. Assume FCPU = 16MHz.
void U0init(unsigned long U0baud){
  unsigned long FCPU = 16000000;
  unsigned int tbaud;
  tbaud = (FCPU / 16 / U0baud - 1);
  // Same as (FCPU / (16 * U0baud)) - 1;
  *myUCSR0A = 0x20;
  *myUCSR0B = 0x18;
  *myUCSR0C = 0x06;
  *myUBRR0  = tbaud;
}

// Read USART0 RDA status bit and return non-zero true if set
unsigned char U0kbhit(){
  if(!(*myUCSR0A & RDA)){
    return false;
  }
  return true;
}

// Read input character from USART0 input buffer
unsigned char U0getchar(){
  unsigned char c = *myUDR0;
  *myUCSR0A &= ~RDA;
  return c;
}

// Wait for USART0 TBE to be set, write char to transmit buffer
void U0putchar(unsigned char U0pdata){
  while(!(*myUCSR0A & TBE));
  *myUDR0 = U0pdata;
  *myUCSR0A &= ~TBE;
}