// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <dht.h>

// port registers
volatile unsigned char* myPORTA = (unsigned char*) 0x22;
volatile unsigned char* myDDRA = (unsigned char*) 0x21;
volatile unsigned char* myPINA = (unsigned char*) 0x20;

volatile unsigned char* myPORTB= (unsigned char*) 0x25;
volatile unsigned char* myDDRB = (unsigned char*) 0x24;
volatile unsigned char* myPINB = (unsigned char*) 0x23;

volatile unsigned char* myPORTE = (unsigned char*) 0x2E;
volatile unsigned char* myDDRE = (unsigned char*) 0x2D;
volatile unsigned char* myPINE = (unsigned char*) 0x2C;

volatile unsigned char* myPORTG = (unsigned char*) 0x34;
volatile unsigned char* myDDRG = (unsigned char*) 0x33;
volatile unsigned char* myPING = (unsigned char*)) 0x32;

volatile unsigned char* myPORTJ = (unsigned char *) 0x105;
volatile unsigned char* myDDRJ = (unsigned char *) 0x104;
volatile unsigned char* myPINJ = (unsigned char *) 0x103;

//timer registers
volatile unsigned int *myTCNT1 = (unsigned int *) 0x84;
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIFR1 = (unsigned char *) 0x36;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;

//adc registers for water sensor
volatile unsigned char* myADCSRA = (unsigned char*) 0x7A;
volatile unsigned char* myADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* myADMUX = (unsigned char*) 0x7C;
volatile unsigned int* myADCDATA = (unsigned int*) 0x78;

// pin change interrupt registers
volatile unsigned char* myPCMSK1 = (unsigned char *) 0x6C;
volatile unsigned char* myPCICR = (unsigned char *) 0x69;

bool button1 = false; // on/off button
bool button2 = false; // vent position button

//dht11 read ready
bool read_dht11 = false;

// humidity sensor
int pinDHT11 = 27;
	//might need the simpleDHT library to set this up

//water sensor
int water_level = 8;
int level_history = 0;

// LED macros and variables
//volatile unsigned char* ledPort = (unsigned char*) 0x##; 
//volatile unsigned char* ledPortDDR  = (unsigned char*) 0x##; 
#define LEDS_OFF()     *ledPort &= ~(0x##);
#define LED_ON(pinNum) *ledPort |= (0x01 << pinNum);

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
int state; // 0=disabled, 1=idle, 2=running, 3=error
char* stateNames[] = {"DISABLED", "IDLE", "RUNNING", "ERROR");

void setup(){  
  //*ledPortDDR |= 0x##;
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

/* Start Button Interrupt:
  setState(1); // idle
*/

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
int threshVal = 0; // value holder
int sensPin = A8; //  sensor pin 

lcd.setCursor(0, 1);

threshVal = analogRead(senspin); // read from analog pin, store in threshVal 

if (thresVal<=300){
  lcd.println("Water level is too low"); // LCD error message
}
//resetTimer(LCD_REFRESH);
  
  
  // setFan(0);   // fan off
  // setState(3); // error
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

void setup()
{ //functions need to be changed to manual port registration 
 pinMode(Pin1, OUTPUT); //define pin for ULN2003 in1 
 pinMode(Pin2, OUTPUT); //define pin for ULN2003 in2   
 pinMode(Pin3, OUTPUT); //define pin for ULN2003 in3   
 pinMode(Pin4, OUTPUT); //define pin for ULN2003 in4    

 pinMode(cwSwitch,INPUT_PULLUP);
 pinMode(ccwSwitch,INPUT_PULLUP);
}

void loop()
{
  if(digitalRead(ccwSwitch) == LOW) 
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
//add reset timer (alternative for delay function) here
  
  void stepMotor(int step)
  {
     digitalWrite(Pin1, pole1[step]);  
     digitalWrite(Pin2, pole2[step]); 
     digitalWrite(Pin3, pole3[step]); 
     digitalWrite(Pin4, pole4[step]); 
  }
