// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <dht.h>

// LED macros and variables
#define LEDS_OFF()     PORTE &= ~(0x3A);
#define LED_ON(pinNum) PORTE |= (0x01 << ledPins[state]);
int ledPins[4] = {2, 3, 5, 1};

// DHT macros and variables
#define DHT_ANALOGPIN A0
#define DHTTYPE DHT11
dht DHT;
#define TEMPERATURE_LIMIT ##

// LCD macros and variables
#define LCD_REFRESH 60 * 1000
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialized w/ #s of interface pins
unsigned long nextLCDRefresh;

// Cooler state variables
volatile int state; // 0=disabled, 1=idle, 2=running, 3=error
const char* stateNames[] = {"DISABLED", "IDLE", "RUNNING", "ERROR");

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
int direction = 0;			   
			    

void setup(){  
  // Interrupt variables
  PCICR |= 1 << PCIE1; // set to generate interrupts
  PCMSK1 |= 1 << PCINT10; // enable on pin 5
  DDRJ &= 0 << DD1; // set to input with pullup enabled
  PORTJ |= 1 << DD1;
	
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
  // If state != 0 & fan adjust input
    // Adjust fan position
    // Serial port: report vent position change
	
if(digitalRead(ccwSwitch) == LOW) //loop for counterclockwise/clockwise buttons
  {
    direction =1;
  }else if(digitalRead(cwSwitch) == LOW)
  {
   direction  = 2;  
  }else
  {
    direction =3; 
  }
 if(direction ==1){ 
   stepperPole++; 
    driveStepper(poleStep);    
 }else if(direction ==2){ 
   stepperPole--; 
    driveStepper(poleStep);    
 }else{
  driveStepper(8);   
 }
 if(stepperPole>7){ 
   stepperPole=0; 
 } 
 if(stepperPole<0){ 
   stepperPole=7; 
 } 
  
  // Temperature > threshold: turn on fan
  if(state = 1 & dht.readTemperature() > TEMPERATURE_LIMIT){
    setFan(1);   // fan on
    setState(2); // running
  }

  // Temperature < threshold: turn off fan
  else if(state = 2 & dht.readTemperature() < TEMPERATURE_LIMIT){
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

// Turns fan motor on and off
void setFan(int fanOn){
  // fanOn = 0 -> off, fanON = 1 -> on
}

// Display temperature and humidity to LCD screen
void displayTempAndHumidity(){
  float humidity = dht.readHumidity(); // read humidity
  float tempF = dht.readTemperature(); // read temp in Farenheit
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
}


/* Stop Button Interrupt:
  setFan(0);   // fan off
  setState(0); // disabled
*/

/* Reset Button Interrupt:
  If waterLevel > threshold
    setState(1); // idle
*/

// NOTE: Can also be done using ADC
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
//resetTimer(LCD_REFRESH);
}  
 



//add reset timer (alternative for delay function) here
  
  void stepMotor(int step)
  {

     digitalWrite(Pin1, pole1[step]);  
     digitalWrite(Pin2, pole2[step]); 
     digitalWrite(Pin3, pole3[step]); 
     digitalWrite(Pin4, pole4[step]); 
  }
