#include <dht.h>

// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>
#include <LiquidCrystal.h>

// LED macros and variables
#define LEDS_OFF()     PORTE &= ~(0x3A);
#define LED_ON(pinNum) PORTE |= (0x01 << ledPins[state]);
int ledPins[4] = {2, 3, 5, 1};

// DHT macros and variables
#define DHT_ANALOGPIN A0
#define DHTTYPE DHT11
DHT dhtSensor;
#define TEMPERATURE_LIMIT 70

// LCD macros and variables
#define LCD_REFRESH 60000
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialized w/ #s of interface pins
unsigned long nextLCDRefresh;

// Cooler state variables
volatile int state; // 0=disabled, 1=idle, 2=running, 3=error
const char stateNames[4][9] = {"DISABLED", "IDLE", "RUNNING", "ERROR"};

// buttons
bool button1 = false; //on button
bool button2 = false; //off button
bool button3 = false; //reset button
bool button4 = false; //step motor input 1
bool button5 = false; //step motor input 2
//**** do we need a vent position button?**********//

// Stepper motor variables
int cwSwitch = 2; //clockwise
int ccwSwitch = 3; //counter clock wise 
int Pin1 = 10; //IN1 
int Pin2 = 11; //IN2
int Pin3 = 12; //IN3
int Pin4 = 13; //IN4
int pole1[] ={0,0,0,0, 0,1,1,1, 0}; //poles each with 8 step vals
int pole2[] ={0,0,0,1, 1,1,0,0, 0};
int pole3[] ={0,1,1,1, 0,0,0,0, 0};
int pole4[] ={1,1,0,0, 0,0,0,1, 0};
int stepperPole = 0;
int poleStep = 0;
int direction = 0;			   
			    
void setup(){  
  // Interrupt variables
  PCICR |= 1 << PCIE1; // set to generate interrupts
  PCMSK1 |= 1 << PCINT10; // enable on pin 5
  DDRJ &= 0 << DD1; // set to input with pullup enabled
  PORTJ |= 1 << DD1;
	
  // Stepper motor setup
  pinMode(Pin1, OUTPUT); //define pin for ULN2003 in1 
  pinMode(Pin2, OUTPUT); //define pin for ULN2003 in2   
  pinMode(Pin3, OUTPUT); //define pin for ULN2003 in3   
  pinMode(Pin4, OUTPUT); //define pin for ULN2003 in4    
  pinMode(cwSwitch,INPUT_PULLUP);
  pinMode(ccwSwitch,INPUT_PULLUP);
  
  DDRE |= 0x3A; // 0b0011_1010, LED outputs
  nextLCDRefresh = 0;
  Serial.begin(9600);
  setState(0); // disabled
  setFan(0);   // fan off
  lcd.begin(16, 2); 
}

void loop(){
  // Adjust fan position
	/*if(state!=0){
    // Get position change direction
    if(digitalRead(ccwSwitch) == LOW) {
      direction =1;
    } else if(digitalRead(cwSwitch) == LOW){
      direction  = 2;  
    } else{
      direction =3; 
    }
    // Change position
    if(direction ==1){ 
      stepperPole++; 
      driveStepper(poleStep);    
    }else if(direction ==2){ 
      stepperPole--; 
      driveStepper(poleStep);    
    }else{
      driveStepper(8);   
    }
    // Account for rollover
    if(stepperPole>7){ 
      stepperPole=0; 
    } else if(stepperPole<0){ 
      stepperPole=7; 
    } 
  }*/
  
  // Temperature > threshold: turn on fan
  if(state = 1 & dhtSensor.readTemperature > TEMPERATURE_LIMIT){
    setFan(1);   // fan on
    setState(2); // running
  }

  // Temperature < threshold: turn off fan
  else if(state = 2 & dhtSensor.readTemperature < TEMPERATURE_LIMIT){
    setFan(0);   // fan off
    setState(1); // idle
  }

  // Every minute: display temp and humidity to LCD screen
  if(millis() >= nextLCDRefresh){
    nextLCDRefresh += LCD_REFRESH;
    if(state != 0){
      displayTempAndHumidity();
    }
  }
}

// Change cooler state
void setState(int newState){
  state = newState;
  //Serial.print(time);
  Serial.print(": state is now ");
  Serial.println(stateNames[newState]); 
  LEDS_OFF();
  LED_ON(newState);
}

// super states
bool on = false;
// substates
bool error_state = false;
bool idle_state = false;
bool running_state = false;

// Turns fan motor on and off
void setFan(int fanOn){
  // fanOn = 0 -> off, fanON = 1 -> on
}

// Display temperature and humidity to LCD screen
void displayTempAndHumidity(){
  float humidity = dhtSensor.readHumidity; // read humidity
  float tempF = dhtSensor.readTemperature; // read temp in Farenheit
  lcd.setCursor(0, 1);

  // Check for failed readings
  if (isnan(humidity) || isnan(tempF)) {
    lcd.println("DHT sensor reading(s) failed");
  }

  // Display humidity and temperature on LCD 
  else {
    lcd.println("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
    lcd.println("Temperature: ");
    lcd.print(tempF);
    lcd.print("F");
  }
}

// Start button interrupt
ISR(PCINT1_vect){ 
  setState(1); // idle
  /* if button 1 is down,
     {
  	button1=true;
     }
     if button 2 is down,
     {
     	button2=true;
     }
     if a button is released, check which button
        if(button1)
	{
		if(running_state)
		{
			// stop fan
			// print time stamp
		}
		//clear display
		clear_lcd();
		//turn yellow LED on and others off
		// change state to off
		on = false;
		running_state = false;
		idle_state = false;
	}
	else if (error_state)
	{
		//turn yellow LED off and red LED on
		// print error message
		// change state to on
		on = true;
	}
	else //if off turn on
	{
		// turn yellow LED off and green LED on
		// change state to on
		on = true;
		idle_state = true;
	}
	button1= false;
  */
}

/* Stop Button Interrupt:
  setFan(0);   // fan off
  setState(0); // disabled
*/

/* Reset Button Interrupt:
  If waterLevel > threshold
    setState(1); // idle
*/

/*
// Comparator Interrupt: // waterLevel < threshold
void waterLevelTest()
{
  int threshVal = 0; // value holder
  int sensPin = A8; //  sensor pin 
  lcd.setCursor(0, 1);
  threshVal = analogRead(senspin); // read from analog pin, store in threshVal 
  if (thresVal<=300){
    lcd.println("Water level is too low"); // LCD error message
  }
}  
 
void stepMotor(int step)
{
  digitalWrite(Pin1, pole1[step]);  
  digitalWrite(Pin2, pole2[step]); 
  digitalWrite(Pin3, pole3[step]); 
  digitalWrite(Pin4, pole4[step]); 
}*/
