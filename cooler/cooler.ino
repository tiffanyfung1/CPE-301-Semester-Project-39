// CPE 301 Semester Project
// Swamp Cooler
// Group 39
// Parker True, Tiffany Fung, Bryce Millis

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <dht.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //library initialized w/ the numbers of interface pins

// Main program constants and variables
#define DHT_ANALOGPIN A0
#define DHTTYPE DHT11
#define LCD_REFRESH 60
#define TEMPERATURE_LIMIT ##
dht DHT;
int state; // 0=disabled, 1=idle, 2=running, 3=error

// I/O Macros
#define LEDS_OFF()     *ledPort &= ~(0x##);
#define LED_ON(pinNum) *ledPort |= (0x01 << pinNum);

void setup(){  
  Serial.begin(9600);
  setState(0); // disabled
  resetTimer(LCD_REFRESH);
  lcd.begin(16, 2); 
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

// Timer Interrupt:
 // if(state != 0){ 
    // Display humidity and temperature on LCD
    float humidity = dht.readHumidity(); // read humidity
    float tempF = dht.readTemperature(); // read temp in Farenheit
    lcd.setCursor(0, 1);
if (isnan(humidity) || isnan(tempF)) { //check for failed readings
  lcd.println("DHT sensor reading(s) failed");
} else {
  lcd.println("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.println("Temperature: ");
  lcd.print(tempF);
  lcd.print("F");
  }
  // resetTimer(LCD_REFRESH);


// NOTE: Can also be done using ADC
// Comparator Interrupt: // waterLevel < threshold
int threshVal = 0; // value holder
int sensPin = A3; //  sensor pin 

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
    dirStatus =1;
  }else if(digitalRead(cwSwitch) == LOW)
  {
   dirStatus  = 2;  
  }else
  {
    dirStatus =3; 
  }
 if(dirStatus ==1){ 
   poleStep++; 
    driveStepper(poleStep);    
 }else if(dirStatus ==2){ 
   poleStep--; 
    driveStepper(poleStep);    
 }else{
  driveStepper(8);   
 }
 if(poleStep>7){ 
   poleStep=0; 
 } 
 if(poleStep<0){ 
   poleStep=7; 
 } 
//add reset timer (alternative for delay function) here
  
  void stepMotor(int step)
  {
     digitalWrite(Pin1, pole1[step]);  
     digitalWrite(Pin2, pole2[step]); 
     digitalWrite(Pin3, pole3[step]); 
     digitalWrite(Pin4, pole4[step]); 
  }
